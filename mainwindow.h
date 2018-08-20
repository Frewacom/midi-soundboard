#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QListWidgetItem>
#include "modalwidget.h"
#include "volumeselectorwidget.h"
#include "midiwrapper.h"
#include "audiowrapper.h"
#include "configwrapper.h"
#include "helpers.h"

// https://github.com/Skycoder42/QHotkey

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
    ConfigWrapper *Settings;
    void OnMidiKeyDown(unsigned int key);
    void OnMidiKeyUp(unsigned int key);

signals:
    void Resized(QSize *newSize);

private slots:
    void on_HeaderButton_clicked();
    void on_StatusControlsVolume_clicked();
    void on_TrackFinished(int reason);
    void on_TrackStarted(TrackInfo *track);
    void on_StatusControlsPausePlay_clicked(bool checked);
    void on_StatusControlsReset_clicked();
    void on_TrackTimer_updated();
    void on_MidiSelectionList_itemClicked(QListWidgetItem *item);
    void on_AudioSelectionList_itemClicked(QListWidgetItem *item);
    void on_MidiRescanButton_clicked();
    void on_AudioRescanButton_clicked();
    void on_AudioVolume_changed(int value, QString name);
    void on_currentProfileLoaded(ProfilePacket *packet);

private:
    Ui::MainWindow *ui;

    QPushButton *_activeHeaderButton = nullptr;
    QMap<QPushButton*, int> _buttonIdentifiers;
    ModalWidget *_volumeModal = nullptr;
    QTimer *_trackTimer;

    void _addMidiDevicesToSelectionList();
    void _addAudioDevicesToSelectionList();
    void _setCurrentPage(QPushButton *button);
    void _populateVolumeModal();

protected:
    void resizeEvent(QResizeEvent* event);
};

#endif // MAINWINDOW_H
