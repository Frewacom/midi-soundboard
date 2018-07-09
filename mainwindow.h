#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QDebug>
#include <irrKlang.h>
#include "midiwrapper.h"

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

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
