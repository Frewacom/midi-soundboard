#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H

#include <QString>

class MidiDevice
{
public:
    MidiDevice(QString name, unsigned int port);

    QString Name;
    unsigned int Port;
};

#endif // MIDIDEVICE_H
