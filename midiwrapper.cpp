#include "midiwrapper.h"

MidiWrapper::MidiWrapper() {
    try {
        this->_midi = new RtMidiIn();
    } catch (RtMidiError &error) {
        Logger::DisplayError(
            QString::fromStdString(error.getMessage())
        );
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
