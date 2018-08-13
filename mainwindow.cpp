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

    // TODO: Put this in helpers.cpp instead
    QFile file(":/styles/main.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    ui->centralWidget->setStyleSheet(styleSheet);

    this->MIDI = new MidiWrapper();
    this->_addDevicesToSelectionList();

    // Set the active screen when starting the application
    this->_activeHeaderButton = ui->HeaderStatusButton;
    this->_activeHeaderButton->setChecked(true);
    ui->DeviceSelectionSaveButton->hide();

    // Associate a header-button with an index to a screen
    this->_buttonIdentifiers.insert(ui->HeaderStatusButton, 0);
    this->_buttonIdentifiers.insert(ui->HeaderDevicesButton, 1);
    this->_buttonIdentifiers.insert(ui->HeaderBindingsButton, 2);
    this->_buttonIdentifiers.insert(ui->HeaderSettingsButton, 3);

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
}

// Callback when a header-button has been clicked (main-nav)
void MainWindow::on_HeaderButton_clicked() {
    if (this->_activeHeaderButton != nullptr) {
        this->_activeHeaderButton->setChecked(false);
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    this->_activeHeaderButton = button;
    ui->ScreenWidget->setCurrentIndex(this->_buttonIdentifiers[button]);
}

// Callback for when the user wants to scan for MIDI-devices
void MainWindow::on_DeviceSelectionRescanButton_clicked() {
    this->MIDI->ScanForMidiDevices();
    this->_addDevicesToSelectionList();
    ui->DeviceSelectionSaveButton->hide();
}

// Adds devices to the QListWidget
void MainWindow::_addDevicesToSelectionList() {
    ui->DeviceSelectionList->clear();

    for (int i = 0; i < this->MIDI->Devices.size(); i++) {
        ui->DeviceSelectionList->addItem(this->MIDI->Devices[i].Name);
    }
}

// When entering the device-selection screen the "Select" button is
// disabled and we want to enable it once we press an item
void MainWindow::on_DeviceSelectionList_itemClicked(QListWidgetItem *item) {
    if (ui->DeviceSelectionSaveButton->isHidden()) {
        ui->DeviceSelectionSaveButton->show();
    }
}

// Callback for when the device-selection-save button is pressed
void MainWindow::on_DeviceSelectionSaveButton_clicked() {
    int selectedDeviceIndex = ui->DeviceSelectionList->currentRow();
    MidiDevice device = this->MIDI->Devices.at(selectedDeviceIndex);

    if (this->MIDI->Connect(&device)) {

    }
}

MainWindow::~MainWindow() {
    delete ui;
}
