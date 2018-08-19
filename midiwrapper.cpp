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
    if (this->_connectedDevice != nullptr) {
        this->Disconnect();
    }

    if (this->_connectedDevice != device) {
        try {
            this->_midi->openPort(device->Port);
            this->_connectedDevice = device;
        } catch (RtMidiError &error) {
            Logger::DisplayError(
                QString::fromStdString(error.getMessage())
            );

            return false;
        }

        return true;
    }
}

QString MidiWrapper::GetChordFromKey(unsigned int key) {
    unsigned int index = key - 21;

    // Some keys gives an remainder of 12-23.
    // Using modulus 12 on 12-23 will give us a number
    // between 0-11, which are valid indexes.
    if (index > 87) {
        return "---";
    }

    return this->_chords[index];
}

void MidiWrapper::Disconnect() {
    this->_midi->closePort();
    this->_connectedDevice = nullptr;
}

MidiWrapper::~MidiWrapper() {
    this->Devices.clear();
    this->_chords.clear();

    this->Disconnect();

    delete this->_midi;
    delete this->_connectedDevice;
}
