#include "midiwrapper.h"

MidiWrapper::MidiWrapper() {
    try {
        this->_midi = new RtMidiIn();
    } catch (RtMidiError &error) {
        Logger::DisplayError(
            QString::fromStdString(error.getMessage())
        );
    }

    this->GetMidiDevices();
}

// Public functions
void MidiWrapper::GetMidiDevices() {
    unsigned int deviceCount = this->_midi->getPortCount();

    // Since we might call this one manually later on in the program
    // we must make sure to clear and resize the vector before using
    if (!this->MidiDevices.empty()) {
        this->MidiDevices.clear();
        this->MidiDevices.resize(deviceCount);
    } else {
        this->MidiDevices.reserve(deviceCount);
    }

    if (deviceCount > 0) {
        for (unsigned int i = 0; i < deviceCount; i++) {
            this->MidiDevices.push_back({
                QString::fromStdString(this->_midi->getPortName(i)),
                i
            });
            Logger::Debug(QString::fromStdString(this->_midi->getPortName(i)));
        }
    } else {
        Logger::Debug("MIDI-device count is 0");
    }
}

bool MidiWrapper::Connect(MidiDevice *device) {
    try {
        this->_midi->openPort(device->Port);
    } catch (RtMidiError &error) {
        Logger::DisplayError(
            QString::fromStdString(error.getMessage())
        );

        return false;
    }

    return true;
}

MidiWrapper::~MidiWrapper() {
    this->MidiDevices.clear();
    delete this->_midi;
}
