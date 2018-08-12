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

    QFile file(":/styles/main.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    ui->centralWidget->setStyleSheet(styleSheet);

    this->MIDI = new MidiWrapper();
    this->_addDevicesToSelectionList();
    this->_activeHeaderButton = ui->HeaderDevicesButton;
    this->_activeHeaderButton->setChecked(true);
    ui->DeviceSelectionSaveButton->setDisabled(true);

    this->_buttonIdentifiers.insert(ui->HeaderDevicesButton, 0);
    this->_buttonIdentifiers.insert(ui->HeaderBindingsButton, 1);
    this->_buttonIdentifiers.insert(ui->HeaderSettingsButton, 2);

    connect(
        ui->HeaderBindingsButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderDevicesButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
    connect(
        ui->HeaderSettingsButton, SIGNAL(clicked()),
        this, SLOT(on_HeaderButton_clicked())
    );
}

void MainWindow::_addDevicesToSelectionList() {
    ui->DeviceSelectionList->clear();

    for (int i = 0; i < this->MIDI->Devices.size(); i++) {
        ui->DeviceSelectionList->addItem(this->MIDI->Devices[i].Name);
    }
}

void MainWindow::on_HeaderButton_clicked() {
    if (this->_activeHeaderButton != nullptr) {
        this->_activeHeaderButton->setChecked(false);
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    this->_activeHeaderButton = button;
    ui->ScreenWidget->setCurrentIndex(this->_buttonIdentifiers[button]);
}

void MainWindow::on_DeviceSelectionList_itemClicked(QListWidgetItem *item) {
    if (!ui->DeviceSelectionSaveButton->isEnabled()) {
        ui->DeviceSelectionSaveButton->setEnabled(true);
    }
}

void MainWindow::on_DeviceSelectionRescanButton_clicked() {
    this->MIDI->ScanForMidiDevices();
    this->_addDevicesToSelectionList();
    ui->DeviceSelectionSaveButton->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}
