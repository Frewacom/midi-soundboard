#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QDateTime>
#include <QListWidgetItem>
#include "modalwidget.h"
#include "volumeselectorwidget.h"
#include "midiwrapper.h"
#include "audiowrapper.h"
#include "helpers.h"

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
    AudioWrapper *Audio;
    void OnMidiKeyDown(unsigned int key);
    void OnMidiKeyUp(unsigned int key);

signals:
    void Resized(QSize *newSize);

private slots:
    void on_DeviceSelectionRescanButton_clicked();
    void on_HeaderButton_clicked();
    void on_DeviceSelectionList_itemClicked(QListWidgetItem *item);
    void on_DeviceSelectionSaveButton_clicked();
    void on_DeviceSelectionAudioList_itemClicked(QListWidgetItem *item);
    void on_StatusControlsVolume_clicked();
    void on_TrackFinished();
    void on_TrackStarted(TrackInfo *track);
    void on_StatusControlsPausePlay_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    QPushButton *_activeHeaderButton = nullptr;
    QMap<QPushButton*, int> _buttonIdentifiers;
    ModalWidget *_volumeModal = nullptr;

    bool _deviceSaveButtonPressed = false;

    void _addDevicesToSelectionList();
    void _enableDeviceSaveButton();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
