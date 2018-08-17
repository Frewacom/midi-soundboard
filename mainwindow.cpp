#include "mainwindow.h"
#include "ui_mainwindow.h"

enum UIScreen {
    DeviceSelection = 0,
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->MIDI = new MidiWrapper();
    this->_addDevicesToSelectionList();

    // Set the active screen when starting the application
    this->_activeHeaderButton = ui->HeaderStatusButton;
    this->_activeHeaderButton->setChecked(true);
    ui->DeviceSelectionSaveButton->setEnabled(false);

    // TODO: Move this to a function so that we can change
    // the pixmap to on/off easily
    QPixmap icon(":/icons/down-arrow-on.svg");
    ui->StatusPressIndicatorDown->setPixmap(icon);

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

    // Create the volume modal and connect it to the resize-event
    // so that its size can update when we resize the MainWindow
    ModalWidget *volumeModal = new ModalWidget(
        new QSize(this->width(), this->height()),
        ui->centralWidget
    );

    volumeModal->setContent(new VolumeSelectorWidget(volumeModal));

    connect(
        this, SIGNAL(Resized(QSize*)),
        volumeModal, SLOT(onResize(QSize*))
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

    Helpers::SetStyleSheet(ui->centralWidget, ":/styles/main.css");
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);

   emit this->Resized(new QSize(this->width(), this->height()));
}
// Callback when a header-button has been clicked (main-nav)
void MainWindow::on_HeaderButton_clicked() {
    if (this->_activeHeaderButton != nullptr) {
        this->_activeHeaderButton->setChecked(false);
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    this->_activeHeaderButton = button;
    ui->ScreenWidget->setCurrentIndex(this->_buttonIdentifiers[button]);

    // We don't need to have the button enabled if they have saved their
    // prefrences, since it will enable again if something changes,
    // and if it doesn't, we don't need to save anything.
    if (this->_deviceSaveButtonPressed) {
        ui->DeviceSelectionSaveButton->setEnabled(false);
        this->_deviceSaveButtonPressed = false;
    }
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

    for (int i = 0; i < this->MIDI->Devices.size(); i++) {
        ui->DeviceSelectionList->addItem(this->MIDI->Devices[i].Name);
    }

    ui->DeviceSelectionAudioList->addItem("test");
    ui->DeviceSelectionAudioList->addItem("trest");
    ui->DeviceSelectionAudioList->addItem("trest");
    ui->DeviceSelectionAudioList->addItem("trest");
    ui->DeviceSelectionAudioList->addItem("trest");
    ui->DeviceSelectionAudioList->addItem("trest");
}

void MainWindow::_enableDeviceSaveButton() {
    if (!ui->DeviceSelectionSaveButton->isEnabled()) {
        if (ui->DeviceSelectionAudioList->selectionModel()->
                selectedIndexes().size() > 0 &&
            ui->DeviceSelectionList->selectionModel()->
                selectedIndexes().size() > 0) {
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
    }
}

void MainWindow::on_StatusControlsVolume_clicked() {

}

MainWindow::~MainWindow() {
    delete ui;
}
