#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->MIDI = new MidiWrapper();
    this->Audio = new AudioWrapper();
    this->_trackTimer = new QTimer(this);
    this->_addDevicesToSelectionList();

    // Set the active screen when starting the application
    this->_activeHeaderButton = ui->HeaderStatusButton;
    this->_activeHeaderButton->setChecked(true);
    ui->DeviceSelectionSaveButton->setEnabled(false);

    // Associate a header-button with an index to a screen
    this->_buttonIdentifiers.insert(ui->HeaderStatusButton, 0);
    this->_buttonIdentifiers.insert(ui->HeaderDevicesButton, 1);
    this->_buttonIdentifiers.insert(ui->HeaderBindingsButton, 2);
    this->_buttonIdentifiers.insert(ui->HeaderSettingsButton, 3);

    ui->HeaderProfileDropdown->addItem("Default");
    ui->HeaderProfileDropdown->addItem("Gaming");
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

    // We want to use the same function for all header-buttons,
    // hence why we connect them manually
    connect(
        ui->HeaderStatusButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderDevicesButton, SIGNAL(clicked()),
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

    // We don't need to have the button enabled if they have saved their
    // prefrences, since it will enable again if something changes,
    // and if it doesn't, we don't need to save anything.
    if (this->_deviceSaveButtonPressed) {
        ui->DeviceSelectionSaveButton->setEnabled(false);
        this->_deviceSaveButtonPressed = false;
    }
}

void MainWindow::_setCurrentPage(QPushButton *button) {
    if (this->_activeHeaderButton != nullptr) {
        this->_activeHeaderButton->setChecked(false);
    }

    this->_activeHeaderButton = button;
    button->setChecked(true);
    ui->ScreenWidget->setCurrentIndex(this->_buttonIdentifiers[button]);
}

// Callback for when the user wants to scan for MIDI-devices
void MainWindow::on_DeviceSelectionRescanButton_clicked() {
    this->MIDI->ScanForMidiDevices();
    this->_addDevicesToSelectionList();
    ui->DeviceSelectionSaveButton->setEnabled(false);
}

// Adds devices to the QListWidget
void MainWindow::_addDevicesToSelectionList() {
    ui->DeviceSelectionList->clear();
    ui->DeviceSelectionAudioList->clear();

    for (int i = 0; i < this->MIDI->Devices.size(); i++) {
        ui->DeviceSelectionList->addItem(this->MIDI->Devices[i].Name);
    }

    for (int j = 0; j < this->Audio->Devices.size(); j++) {
        ui->DeviceSelectionAudioList->addItem(this->Audio->Devices[j].Name);
    }
}

void MainWindow::_enableDeviceSaveButton() {
    if (!ui->DeviceSelectionSaveButton->isEnabled()) {
        if (ui->DeviceSelectionAudioList->selectionModel()->
                selectedIndexes().size() > 0 &&
            ui->DeviceSelectionList->selectionModel()->
                selectedIndexes().size() > 0)
        {
            ui->DeviceSelectionSaveButton->setEnabled(true);
        }
    }
}

// When entering the device-selection screen the "Select" button is
// disabled and we want to enable it once we press an item
void MainWindow::on_DeviceSelectionList_itemClicked(QListWidgetItem *item) {
    this->_enableDeviceSaveButton();
}

void MainWindow::on_DeviceSelectionAudioList_itemClicked(QListWidgetItem *item)
{
    if (ui->DeviceSelectionAudioList->selectionModel()->
        selectedIndexes().size() > 2)
    {
        QList<QModelIndex> selections = ui->DeviceSelectionAudioList->
            selectionModel()->selectedIndexes();

        ui->DeviceSelectionAudioList->selectionModel()->
            select(selections.first(),QItemSelectionModel::Deselect);
    }

    this->_enableDeviceSaveButton();
}

// Callback for when the device-selection-save button is pressed
void MainWindow::on_DeviceSelectionSaveButton_clicked() {
    int selectedDeviceIndex = ui->DeviceSelectionList->currentRow();
    MidiDevice device = this->MIDI->Devices.at(selectedDeviceIndex);
    this->_deviceSaveButtonPressed = true;

    if (this->MIDI->Connect(&device)) {
        qDebug() << "Conected to " << device.Name;
        this->_setCurrentPage(ui->HeaderStatusButton);
    }

    QModelIndexList indexes = ui->DeviceSelectionAudioList->selectionModel()->selectedIndexes();
    for (int i = 0; i < indexes.size(); i++) {
        int index = indexes.at(i).row();

        if (!this->Audio->Connect(&this->Audio->Devices.at(index))) {
            Logger::DisplayError(
                "Could not connect to AudioDevice: " + this->Audio->Devices.at(index).Name
            );
        }
    }
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
    ui->StatusPressedKey->setText("KEY " + QString::number(key));
    ui->StatusPressedChord->setText(this->MIDI->GetChordFromKey(key));

    if (key == 24) {
        this->Audio->StartPlayback();
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

MainWindow::~MainWindow() {
    delete ui;
}
