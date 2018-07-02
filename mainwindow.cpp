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

    //this->m_engine = createIrrKlangDevice();
    //RtMidiIn *in = new RtMidiIn();
    //qDebug() << in->getPortCount();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//void MainWindow::on_playSong_clicked()
//{
    //QDir music(QCoreApplication::applicationDirPath());

    //this->m_engine->play2D(music.filePath("music/shootingstars.wav").toStdString().c_str());
//}
