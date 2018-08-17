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
    QString GetChordFromKey(unsigned int key);

    // We need this to public atm so that we can
    // access the setCallback method from main.cpp.
    // We should - later on - create a function in MidiWrapper
    // which does this for us without exposing the RtMidi object
     RtMidiIn *_midi;

signals:
    void on_keyDown();
    void on_keyUp();

private:
    MidiDevice *_connectedDevice;
    QList<QString> _chords = {
        "C","C#","D","D#","E","F",
        "F#","G","G#","A","A#","B"
    };
};

#endif // MIDIWRAPPER_H
