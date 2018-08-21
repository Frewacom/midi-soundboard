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
    bool Connect(MidiDevice *device);
    void Disconnect();
    QString GetChordFromKey(unsigned int key);
    MidiDevice* GetDeviceByName(QString name);

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
        "A0", "A0#", "B0",
        "C1", "C1#", "D1", "D1#", "E1", "F1",
        "F1#", "G1", "G1#", "A1", "A1#", "B1",
        "C2", "C2#", "D2", "D2#", "E2", "F2",
        "F2#", "G2", "G2#", "A2", "A2#", "B2",
        "C3", "C3#", "D3", "D3#", "E3", "F3",
        "F3#", "G3", "G3#", "A3", "A3#", "B3",
        "C4", "C4#", "D4", "D4#", "E4", "F4",
        "F4#", "G4", "G4#", "A4", "A4#", "B4",
        "C5", "C5#", "D5", "D5#", "E5", "F5",
        "F5#", "G5", "G5#", "A5", "A5#", "B5",
        "C6", "C6#", "D6", "D6#", "E6", "F6",
        "F6#", "G6", "G6#", "A6", "A6#", "B6",
        "C7", "C7#", "D7", "D7#", "E7", "F7",
        "F7#", "G7", "G7#", "A7", "A7#", "B7",
        "C8"
    };
};

#endif // MIDIWRAPPER_H
