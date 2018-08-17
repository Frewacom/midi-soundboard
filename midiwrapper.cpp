#include "midiwrapper.h"

void MidiCallback(double delta, std::vector<unsigned char> *message, void *data) {
    if (message->size() >= 3) {
        unsigned int status = message->at(0);
        unsigned int key = message->at(1);
        unsigned int data = message->at(2);

        if (status == MidiStatus::KeyDown) {

        } else if (status == MidiStatus::KeyUp) {

        }
    }
}

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
        // TODO: Move to a Disconnect function
        this->_midi->closePort();
        this->_connectedDevice = nullptr;
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

        this->_midi->setCallback(&MidiCallback);
        return true;
    }
}

MidiWrapper::~MidiWrapper() {
    this->Devices.clear();
    delete this->_midi;
}
