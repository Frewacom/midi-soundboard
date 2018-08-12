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

    this->ScanForMidiDevices();
}

// Public functions
void MidiWrapper::ScanForMidiDevices() {
    unsigned int deviceCount = this->_midi->getPortCount();

    // This will cause problems if one device is removed and another one
    // added between launch and doing a rescan
    if (deviceCount != this->Devices.size()) {
        // Since we might call this one manually later on in the program
        // we must make sure to clear and resize the vector before using
        this->Devices.clear();

        if (!this->Devices.empty()) {
            this->Devices.resize(deviceCount);
        } else {
            this->Devices.reserve(deviceCount);
        }

        for (unsigned int i = 0; i < deviceCount; i++) {
            this->Devices.push_back({
                QString::fromStdString(this->_midi->getPortName(i)),
                i
            });
        }
    }
}

MidiWrapper::~MidiWrapper() {
    delete this->_midi;
}
