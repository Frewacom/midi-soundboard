#include "mainwindow.h"
#include <QApplication>

// Absolutely disgusting
MainWindow *w;

void MidiCallback(double delta, std::vector<unsigned char> *message, void *data) {
    if (message->size() >= 3) {
        unsigned int status = message->at(0);
        unsigned int key = message->at(1);
        unsigned int data = message->at(2);

        qDebug() << key;

        if (status == MidiStatus::KeyDown) {
            w->OnMidiKeyDown(key);
        } else if (status == MidiStatus::KeyUp) {
            w->OnMidiKeyUp(key);
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow();
    w->MIDI->_midi->setCallback(&MidiCallback);
    w->show();

    return a.exec();
}
