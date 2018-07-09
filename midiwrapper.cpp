#include "midiwrapper.h"

MidiWrapper::MidiWrapper() {
    try {
        this->_midi = new RtMidiIn();
    } catch (RtMidiError &error) {
        Logger::DisplayError(
            QString::fromStdString(error.getMessage())
        );

        // When the logging dialog has been implemented,
        // this should be removed and be ran when the dialog
        // has been closed
        exit(EXIT_FAILURE);
    }

    this->GetMidiDevices();
}

// Public functions
void MidiWrapper::GetMidiDevices() {
    unsigned int deviceCount = this->_midi->getPortCount();

    // Since we might call this one manually later on in the program
    // we must make sure to clear and resize the vector before using
    if (!this->_midiDevices.empty()) {
        this->_midiDevices.clear();
        this->_midiDevices.resize(deviceCount);
    } else {
        this->_midiDevices.reserve(deviceCount);
    }

    for (unsigned int i = 0; i < deviceCount; i++) {
        this->_midiDevices.push_back({
            QString::fromStdString(this->_midi->getPortName(i)),
            i
        });
    }
}

MidiWrapper::~MidiWrapper() {
    delete this->_midi;
}
