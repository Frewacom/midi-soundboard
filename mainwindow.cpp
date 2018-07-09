#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QFile file(":/styles/main.css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    ui->centralWidget->setStyleSheet(styleSheet);

    //this->MIDI = new MidiWrapper();
}

MainWindow::~MainWindow()
{
    delete ui;
}

