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
            QJsonDocument profileJSON = Helpers::GetFileJSONContents(path);
            this->_currentProfile = profileJSON.object();

            this->_currentProfilePacket = this->_packetProfileJSON(
                profile.toString(),
                this->_currentProfile
            );
        }
    }
}

int ConfigWrapper::GetProfiles() {
    QDir profileDir(this->_applicationFolder.filePath("config/profiles"));
    QDirIterator iterator(
        profileDir.absolutePath(),
        QStringList() << "*.json", QDir::Files,
        QDirIterator::NoIteratorFlags
    );

    int count = 0;
    while (iterator.hasNext()) {
        QFile file(iterator.next());
        QJsonDocument profile = Helpers::GetFileJSONContents(&file);
        this->_packetProfileJSON(file.fileName(), profile.object());
        count++;
    }

    return count;
}

ProfilePacket* ConfigWrapper::GetProfileByName(QString name) {
    auto it = this->_profilePackets.find(name);
    if (it != this->_profilePackets.end()) {
        return it.value();
    }
}

void ConfigWrapper::SetCurrentProfile(ProfilePacket *packet) {
    this->_currentProfilePacket = packet;
}

ProfilePacket* ConfigWrapper::_packetProfileJSON(QString fileName, QJsonObject profile) {
    ProfilePacket *packet = new ProfilePacket();
    packet->FileName = fileName;

    QJsonValue name = profile["name"];
    QJsonValue midiDevice = profile["midi_device"];
    QJsonValue audioDevices = profile["audio_devices"];
    QVariantMap profileMap = profile.toVariantMap();
    QVariantMap bindings = profileMap["bindings"].toMap();

    // TODO: We probably want to check if its empty
    // - and insert some placeholder name instead
    if (name.isString()) {
        packet->Name = name.toString();
        emit this->on_Profile_found(name.toString());
    }
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

    for(int i = 0; i < bindings.keys().count(); i++){
        QString key = bindings.keys().at(i);
        QString binding = bindings[key.toLocal8Bit()].toString();
        packet->Bindings.insert(key.toInt(), binding);
    }

    this->_profilePackets.insert(name.toString(), packet);

    return packet;
}

ProfilePacket* ConfigWrapper::GetCurrentProfile() {
    return this->_currentProfilePacket;
}

void ConfigWrapper::UpdateDeviceVolume(QString name, int volume) {

}

QString ConfigWrapper::GetBinding(int key) {
    auto binding = this->_currentProfilePacket->Bindings.find(key);
    if (binding != this->_currentProfilePacket->Bindings.end()) {
        return (QString)binding.value();
    }

    return QString();
}

void ConfigWrapper::SaveBinding(int key, QString action) {
    this->_currentProfilePacket->Bindings[key] = action;
}

void ConfigWrapper::SaveProfile() {
    ProfilePacket *packet = this->_currentProfilePacket;
    QString path = this->_applicationFolder.filePath(
        "config/profiles/" + this->_currentProfilePacket->FileName
    );

    QJsonObject obj;
    QJsonArray audioDevices;
    QJsonObject bindings;
    QMapIterator<int, QString> it(packet->Bindings);

    for (int i = 0; i < packet->AudioDevices.size(); i++) {
        QJsonObject device;
        device.insert("name", packet->AudioDevices.at(i)->Name);
        device.insert("volume", packet->AudioDevices.at(i)->Volume);
        audioDevices.append(device);
    }

    while (it.hasNext()) {
        it.next();
        bindings.insert(QString::number(it.key()), it.value());
    }

    obj["name"] = packet->Name;
    obj["midi_device"] = packet->MidiDevice;
    obj["audio_devices"] = audioDevices;
    obj["bindings"] = bindings;

    QJsonDocument doc(obj);
    QString json = doc.toJson();
    QFile profile(path);

    if (profile.open(QIODevice::WriteOnly)){
        profile.write(json.toUtf8());
        profile.close();
    } else {
        qDebug() << "Could not write to " << path;
    }
}

void ConfigWrapper::SaveMidiDevice(QString device) {
    this->_currentProfilePacket->MidiDevice = device;
    this->SaveProfile();
}

ConfigWrapper::~ConfigWrapper() {
    this->SaveProfile();
}
