#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->MIDI = new MidiWrapper();
    this->Audio = new AudioWrapper();
    this->Settings = new ConfigWrapper();

    this->_trackTimer = new QTimer(this);
    this->_addMidiDevicesToSelectionList();
    this->_addAudioDevicesToSelectionList();

    // Set the active screen when starting the application
    this->_activeHeaderButton = ui->HeaderStatusButton;
    this->_activeHeaderButton->setChecked(true);

    // Associate a header-button with an index to a screen
    this->_buttonIdentifiers.insert(ui->HeaderStatusButton, 0);
    this->_buttonIdentifiers.insert(ui->HeaderMidiButton, 1);
    this->_buttonIdentifiers.insert(ui->HeaderAudioButton, 2);
    this->_buttonIdentifiers.insert(ui->HeaderBindingsButton, 3);
    this->_buttonIdentifiers.insert(ui->HeaderSettingsButton, 4);

    ui->HeaderProfileDropdown->view()->window()->setWindowFlags(
        Qt::Popup | Qt::FramelessWindowHint |
        Qt::NoDropShadowWindowHint
    );

    // Connect the Track-timer
    connect(
        this->_trackTimer, SIGNAL(timeout()),
        this, SLOT(on_TrackTimer_updated())
    );

    // Create the volume modal and connect it to the resize-event
    // so that its size can update when we resize the MainWindow
   this->_volumeModal = new ModalWidget(
        new QSize(this->width(), this->height()),
        new VolumeSelectorWidget(this->_volumeModal),
        ui->centralWidget
    );

    connect(
        this, SIGNAL(Resized(QSize*)),
        this->_volumeModal, SLOT(onResize(QSize*))
    );

    connect(
        this->_volumeModal->ContentWidget,
        SIGNAL(volumeChanged(int, QString)),
        this,
        SLOT(on_AudioVolume_changed(int, QString))
    );

    // We want to use the same function for all header-buttons,
    // hence why we connect them manually
    connect(
        ui->HeaderStatusButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderMidiButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderAudioButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderBindingsButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderSettingsButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );

    // Audio signals
    connect(
        this->Audio, SIGNAL(TrackStarted(TrackInfo*)),
        this, SLOT(on_TrackStarted(TrackInfo*))
    );

    connect(
        this->Audio, SIGNAL(TrackFinished(int)),
        this, SLOT(on_TrackFinished(int))
    );

    // Config signals
    connect(
        this->Settings, SIGNAL(on_Profile_found(QString)),
        this, SLOT(on_ProfileFound(QString))
    );

    // Apply current profile - if one is chosen
    if (this->Settings->GetProfiles() == 0) {
        ui->HeaderProfileDropdown->addItem("No profiles");
        ui->HeaderProfileDropdown->setCurrentIndex(0);
        ui->HeaderProfileDropdown->setDisabled(true);
    }

    ProfilePacket *packet = this->Settings->GetCurrentProfile();
    if (packet) {
        this->_applyProfile(packet);
    }

    connect(
        ui->HeaderProfileDropdown, SIGNAL(currentIndexChanged(int)),
        this, SLOT(on_HeaderProfileDropdownChanged(int))
    );

    Helpers::SetStyleSheet(ui->centralWidget, ":/styles/main.css");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   emit this->Resized(new QSize(this->width(), this->height()));
}
// Callback when a header-button has been clicked (main-nav)
void MainWindow::on_HeaderButton_clicked() {
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    this->_setCurrentPage(button);
}

void MainWindow::_setCurrentPage(QPushButton *button) {
    if (this->_activeHeaderButton != nullptr) {
        this->_activeHeaderButton->setChecked(false);
    }

    this->_activeHeaderButton = button;
    button->setChecked(true);
    ui->ScreenWidget->setCurrentIndex(this->_buttonIdentifiers[button]);
}

// Profile dropdown callback
void MainWindow::on_HeaderProfileDropdownChanged(int index) {
    ProfilePacket *profile = this->Settings->GetProfileByName(
        ui->HeaderProfileDropdown->currentText()
    );

    this->_applyProfile(profile);
}

// Config functions
void MainWindow::_applyProfile(ProfilePacket *packet) {
    this->Audio->StopPlayback();

    ui->HeaderProfileDropdown->setCurrentText(packet->Name);

    MidiDevice *midiDevice = this->MIDI->GetDeviceByName(packet->MidiDevice);
    if (midiDevice != nullptr) {
        this->MIDI->Connect(midiDevice);
        this->_selectItemsByText(ui->MidiSelectionList, midiDevice->Name);
    }

    if (packet->AudioDevices.size() > 0) {
        QList<QString> *audioDeviceNames = new QList<QString>();
         this->Audio->Engines.clear();

        for (int i = 0; i < packet->AudioDevices.size(); i++) {
            AudioDevicePacket *audioDevice = packet->AudioDevices.at(i);
            audioDeviceNames->append(audioDevice->Name);
            this->Audio->Connect(audioDevice->Name);
            this->Audio->SetVolume(audioDevice->Volume, audioDevice->Name);
        }

        this->_selectItemsByText(ui->AudioSelectionList, audioDeviceNames);
        this->_populateVolumeModal();
    }

    this->Settings->SetCurrentProfile(packet);
}

void MainWindow::_selectItemsByText(QListWidget *widget, QString name) {
    for (int i = 0; i < widget->count(); i++) {
        if (widget->item(i)->text() == name) {
            widget->item(i)->setSelected(true);
        }
    }
}

void MainWindow::_selectItemsByText(QListWidget *widget, QList<QString> *names) {
    for (int i = 0; i < widget->count(); i++) {
        for (int j = 0; j < names->size(); j++) {
            if (widget->item(i)->text() == names->at(j)) {
                widget->item(i)->setSelected(true);
            }
        }
    }
}

void MainWindow::on_ProfileFound(QString name) {
    ui->HeaderProfileDropdown->addItem(name);
}

// Callback for when the user wants to scan for MIDI-devices
void MainWindow::on_MidiRescanButton_clicked() {
    this->MIDI->ScanForMidiDevices();
    this->_addMidiDevicesToSelectionList();
    this->MIDI->Disconnect();
}

void MainWindow::on_AudioRescanButton_clicked() {
    this->Audio->ScanForAudioDevices();
    this->_addAudioDevicesToSelectionList();
    this->Audio->DisconnectAll();
}

void MainWindow::_addMidiDevicesToSelectionList() {
    ui->MidiSelectionList->clear();

    for (int i = 0; i < this->MIDI->Devices.size(); i++) {
        ui->MidiSelectionList->addItem(this->MIDI->Devices[i].Name);
    }
}

void MainWindow::_addAudioDevicesToSelectionList() {
    ui->AudioSelectionList->clear();

    for (int j = 0; j < this->Audio->Devices.size(); j++) {
        ui->AudioSelectionList->addItem(this->Audio->Devices[j].Name);
    }
}

void MainWindow::on_MidiSelectionList_itemClicked(QListWidgetItem *item) {
    int selectedDeviceIndex = ui->MidiSelectionList->currentRow();
    MidiDevice device = this->MIDI->Devices.at(selectedDeviceIndex);

    this->MIDI->Connect(&device);
    this->Settings->SaveMidiDevice(device.Name);
}

void MainWindow::on_AudioSelectionList_itemClicked(QListWidgetItem *item)
{
    QList<QModelIndex> selections = ui->AudioSelectionList->
        selectionModel()->selectedIndexes();

    if (ui->AudioSelectionList->selectionModel()->
        selectedIndexes().size() > 2)
    {
        ui->AudioSelectionList->selectionModel()->
            select(selections.first(), QItemSelectionModel::Deselect);

        this->Audio->Disconnect(selections.first().data().toString());
    }

    if (item->isSelected()) {
        if (!this->Audio->Connect(item->text())) {
            Logger::DisplayError(
                "Could not connect to audio device: " + item->text()
            );
        }
    } else {
        this->Audio->Disconnect(item->text());
    }

    this->_populateVolumeModal();
}

void MainWindow::_populateVolumeModal() {
    VolumeSelectorWidget *volumeWidget =
        (VolumeSelectorWidget*)this->_volumeModal->ContentWidget;
    volumeWidget->SetDevices(&this->Audio->Engines);
}

// Status screen
void MainWindow::on_StatusControlsVolume_clicked() {
    this->_volumeModal->open();
}

void MainWindow::on_StatusControlsPausePlay_clicked(bool checked) {
    if (this->Audio->CurrentTrack != nullptr) {
        if (!this->Audio->CurrentTrack->isFinished()) {
            if (!checked) {
                qDebug() << "Pause";
                this->Audio->Pause();
                this->_trackTimer->stop();
            } else {
                this->Audio->Play();
                this->_trackTimer->start(10);
            }
        }
    } else {
        ui->StatusControlsPausePlay->setChecked(false);
    }
}

void MainWindow::on_StatusControlsReset_clicked() {
    if (this->Audio->CurrentTrack != nullptr) {
        this->Audio->StopPlayback(PlaybackStopReason::StopButton);
        this->_trackTimer->stop();
    }
}


// MIDI callbacks
void MainWindow::OnMidiKeyDown(unsigned int key) {
    if (this->_pressedKey != key) {
        ui->StatusPressedKey->setText("KEY " + QString::number(key));
        ui->StatusPressedChord->setText(this->MIDI->GetChordFromKey(key));
        this->_pressedKey = key;
    }

    QString action = this->Settings->GetBinding(key);
    if (!action.isEmpty()) {
        if (action == "STOP") {
            this->Audio->StopPlayback(PlaybackStopReason::StopButton);
        } else if (action == "PAUSE") {
            if (this->Audio->CurrentTrack != nullptr && !this->Audio->IsPaused) {
                this->Audio->Pause();
            }
        } else if (action == "PAUSE_COMBINED") {
            if (this->Audio->CurrentTrack != nullptr) {
                if (this->Audio->IsPaused) {
                    this->Audio->Play();
                } else {
                    this->Audio->Pause();
                }
            }
        } else if (action == "RESUME") {
            if (this->Audio->CurrentTrack != nullptr && this->Audio->IsPaused) {
                this->Audio->Play();
            }
        } else {
            this->Audio->StartPlayback(action);
        }
    }
}

void MainWindow::OnMidiKeyUp(unsigned int key) {

}

// Audio callbacks
void MainWindow::on_TrackStarted(TrackInfo *track) {
    ui->StatusControlsSong->setText(track->Name);
    ui->StatusControlsPausePlay->setChecked(true);
    ui->StatusControlsTimeLength->setText(
        QDateTime::fromMSecsSinceEpoch(track->Length).toUTC().toString("mm:ss")
    );
    ui->StatusControlsTimeCurrent->setText("00:00");
    ui->StatusControlsTimebar->setValue(0);
    ui->StatusControlsTimebar->setRange(0, track->Length);

    this->_trackTimer->start(10);
}

void MainWindow::on_TrackFinished(int reason) {
    this->_trackTimer->stop();

    if (reason != PlaybackStopReason::Overridden) {
        ui->StatusControlsSong->setText("Nothing");
        ui->StatusControlsPausePlay->setChecked(false);
        ui->StatusControlsTimeLength->setText("00:00");
        ui->StatusControlsTimeCurrent->setText("00:00");
        ui->StatusControlsTimebar->setValue(0);
    }
}

void MainWindow::on_TrackTimer_updated() {
    int currentPosition = this->Audio->CurrentTrack->getPlayPosition();

    if (currentPosition != -1) {
        ui->StatusControlsTimebar->setValue(currentPosition);
        ui->StatusControlsTimeCurrent->setText(
            QDateTime::fromMSecsSinceEpoch(currentPosition).toUTC().toString("mm:ss")
        );
    }
}

void MainWindow::on_AudioVolume_changed(int value, QString name) {
    this->Audio->SetVolume(value, name);
    this->Settings->UpdateDeviceVolume(name, value);
}

MainWindow::~MainWindow() {
    // TODO: Save all data when we quit
    delete ui;
}

