#ifndef MIDIWRAPPER_H
#define MIDIWRAPPER_H

#include <QString>
#include <RtMidi.h>
#include "logger.h"

struct MidiDevice {
    QString Name;
    unsigned int Port;
};

class MidiWrapper
{
public:
    MidiWrapper();
    ~MidiWrapper();

    std::vector<MidiDevice> Devices;

    void ScanForMidiDevices();
    void GetMidiDevices();
    bool Connect(MidiDevice *device);

private:
    // Functions

    // Variables
    RtMidiIn *_midi;
};

#endif // MIDIWRAPPER_H
