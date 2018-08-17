#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QMap>
#include <QDebug>
#include <QPixmap>
#include <irrKlang.h>
#include <QListWidgetItem>
#include "modalwidget.h"
#include "volumeselectorwidget.h"
#include "midiwrapper.h"
#include "helpers.h"

using namespace irrklang;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    MidiWrapper *MIDI;

signals:
    void Resized(QSize *newSize);

private slots:
    void on_DeviceSelectionRescanButton_clicked();
    void on_HeaderButton_clicked();
    void on_DeviceSelectionList_itemClicked(QListWidgetItem *item);
    void on_DeviceSelectionSaveButton_clicked();
    void on_DeviceSelectionAudioList_itemClicked(QListWidgetItem *item);

    void on_StatusControlsVolume_clicked();

private:
    Ui::MainWindow *ui;

    QPushButton *_activeHeaderButton = nullptr;
    QMap<QPushButton*, int> _buttonIdentifiers;

    bool _deviceSaveButtonPressed = false;
    ModalWidget *_volumeModal = nullptr;

    void _addDevicesToSelectionList();
    void _enableDeviceSaveButton();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
