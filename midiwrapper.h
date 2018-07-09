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

    void GetMidiDevices();

private:
    // Functions

    // Variables
    RtMidiIn *_midi;
    std::vector<MidiDevice> _midiDevices;
};

#endif // MIDIWRAPPER_H
