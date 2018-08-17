#ifndef MIDIWRAPPER_H
#define MIDIWRAPPER_H

#include <QString>
#include <RtMidi.h>
#include "logger.h"

struct MidiDevice {
    QString Name;
    unsigned int Port;
};

enum MidiStatus {
    KeyDown = 144,
    KeyUp = 128
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
    RtMidiIn *_midi;
    MidiDevice *_connectedDevice;
};

#endif // MIDIWRAPPER_H
