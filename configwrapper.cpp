#include "configwrapper.h"

ConfigWrapper::ConfigWrapper(QObject *parent) : QObject(parent) {
    this->_applicationFolder = Helpers::GetApplicationPath();
    this->_settingsPath = this->_applicationFolder.filePath("config/settings.json");

    if (Helpers::CheckIfFileExists(this->_settingsPath)) {
        this->_settingsContents = Helpers::GetFileJSONContents(this->_settingsPath);
        this->_settingsJSON = this->_settingsContents.object();
        this->_parseSettingsJSON();
    }
}

void ConfigWrapper::_parseSettingsJSON() {
    QJsonValue profile = this->_settingsJSON["current_profile"];
    if (!profile.isNull() && profile.isString()) {
        QString path = this->_applicationFolder.filePath("config/profiles/" + profile.toString());

        if (Helpers::CheckIfFileExists(path)) {
            QJsonDocument profile = Helpers::GetFileJSONContents(path);
            this->_currentProfile = profile.object();

            this->_currentProfilePacket = this->_packetProfileJSON(
                this->_currentProfile
            );
        }
    }
}

ProfilePacket* ConfigWrapper::_packetProfileJSON(QJsonObject profile) {
    ProfilePacket *packet = new ProfilePacket();

    QJsonValue name = profile["name"];
    QJsonValue midiDevice = profile["midi_device"];
    QJsonValue audioDevices = profile["audio_devices"];
    QJsonValue bindings = profile["bindings"];

    // TODO: We probably want to check if its empty
    // - and insert some placeholder name instead
    if (name.isString()) packet->Name = name.toString();
    if (midiDevice.isString()) packet->MidiDevice = midiDevice.toString();
    if (!audioDevices.isNull()) {
        QJsonArray deviceArray = audioDevices.toArray();

        foreach (const QJsonValue &value, deviceArray) {
            AudioDevicePacket *devicePacket = new AudioDevicePacket();

            QJsonValue deviceName = value["name"];
            QJsonValue deviceVolume = value["volume"];

            if (deviceName.isString()) devicePacket->Name = deviceName.toString();
            if (!deviceVolume.isNull()) devicePacket->Volume = deviceVolume.toString().toInt();

            packet->AudioDevices.append(devicePacket);
        }
    }

    return packet;
}

ProfilePacket* ConfigWrapper::GetCurrentProfile() {
    return this->_currentProfilePacket;
}

void ConfigWrapper::UpdateDeviceVolume(QString name, int volume) {

}

ConfigWrapper::~ConfigWrapper() {

}
