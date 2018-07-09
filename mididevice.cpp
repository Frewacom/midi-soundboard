#include "mididevice.h"

MidiDevice::MidiDevice(QString name, unsigned int port)
{
    this->Name = name;
    this->Port = port;
}
