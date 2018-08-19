#include "audiowrapper.h"

AudioWrapper::AudioWrapper(QObject *parent) : QObject(parent) {
    this->ScanForAudioDevices();
    this->_receiver = new SoundReceiver(
        std::bind(&AudioWrapper::TrackFinishedCallback, this)
    );
}

void AudioWrapper::ScanForAudioDevices() {
    ISoundDeviceList *devices = createSoundDeviceList();
    int deviceCount = devices->getDeviceCount();

    if (deviceCount != this->Devices.size()) {
        this->Devices.clear();

        if (!this->Devices.empty()) {
            this->Devices.resize(deviceCount);
        } else {
            this->Devices.reserve(deviceCount);
        }

        for (int i = 0; i < deviceCount; i++) {
            this->Devices.push_back({
                QString::fromLatin1(devices->getDeviceDescription(i)),
                devices->getDeviceID(i)
            });
        }
    }
}

bool AudioWrapper::Connect(QString name) {
    AudioDevice *device = this->_getAudioDeviceByName(name);

    if (device != nullptr) {
        ISoundEngine *engine = createIrrKlangDevice(
            ESOD_AUTO_DETECT,
            ESEO_DEFAULT_OPTIONS,
            device->Id
        );

        if (engine) {
            this->Engines.push_back({
                device,
                engine
            });

            return true;
        } else {
            qDebug() << "Could not create engine: " + name;
            return false;
        }
    } else {
        qDebug() << "Could not find device: " + name;
        return false;
    }
}

void AudioWrapper::StartPlayback() {
    QDir path = Helpers::GetApplicationPath();

    if (this->_stopReason != PlaybackStopReason::None) {
        if (this->CurrentTrack != nullptr) {
            this->_stopReason = PlaybackStopReason::Overridden;
        } else {
            this->_stopReason = PlaybackStopReason::Default;
        }
    }

    if (this->Engines.size() > 0) {
        this->StopPlayback(this->_stopReason);

        for (int i = 0; i < this->Engines.size(); i++) {
            // TODO: Make sure we only assign CurrentTrack once
            this->CurrentTrack = this->Engines.at(i).Engine->play2D(
                path.filePath("music/boosted-animal.wav").toStdString().c_str(),
                false,
                false,
                true
            );
        }

        this->CurrentTrack->setSoundStopEventReceiver(this->_receiver);

        // Track name should be extracted from the path
        TrackInfo *track = new TrackInfo();
        track->Name ="boosted-animal.wav";
        track->Length = this->CurrentTrack->getPlayLength();
        emit this->TrackStarted(track);
    }
}

void AudioWrapper::StopPlayback(int reason) {
    this->_stopReason = reason;

    for (int i = 0; i < this->Engines.size(); i++) {
        this->Engines.at(i).Engine->stopAllSounds();
    }
}

void AudioWrapper::Pause() {
    for (int i = 0; i < this->Engines.size(); i++) {
        this->Engines.at(i).Engine->setAllSoundsPaused();
    }
}

void AudioWrapper::Play() {
    for (int i = 0; i < this->Engines.size(); i++) {
        this->Engines.at(i).Engine->setAllSoundsPaused(false);
    }
}

void AudioWrapper::SetVolume(int volume, QString deviceName) {
    AudioEngine *device = this->_getAudioEngineByName(deviceName);

    if (device != nullptr) {
        device->Engine->setSoundVolume((float)volume / (float)100);
    }
}

void AudioWrapper::TrackFinishedCallback() {
    this->CurrentTrack = nullptr;
    this->_stopReason = PlaybackStopReason::None;

    emit this->TrackFinished(this->_stopReason);
}

AudioDevice* AudioWrapper::_getAudioDeviceByName(QString name) {
    for (int i = 0; i < this->Devices.size(); i++) {
        if (this->Devices.at(i).Name == name) {
            return &this->Devices.at(i);
        }
    }

    return nullptr;
}

AudioEngine* AudioWrapper::_getAudioEngineByName(QString name) {
    for (int i = 0; i < this->Engines.size(); i++) {
        if (this->Engines.at(i).Data->Name == name) {
            return &this->Engines.at(i);
        }
    }

    return nullptr;
}

void AudioWrapper::Disconnect(QString name) {
    for (int i = 0; i < this->Engines.size(); i++) {
        if (this->Engines.at(i).Data->Name == name) {
            this->Engines.at(i).Engine->drop();
            this->Engines.erase(this->Engines.begin() + i);
            qDebug() << "Disconnected engine";
            return;
        }
    }

    qDebug() << "Could not disconnect engine: " + name;
}

void AudioWrapper::DisconnectAll() {
    for (int i = 0; i < this->Engines.size(); i++) {
        this->Engines.at(i).Engine->drop();
        this->Engines.erase(this->Engines.begin() + i);
    }
}

AudioWrapper::~AudioWrapper() {
    this->DisconnectAll();

    this->Engines.clear();
    this->Devices.clear();
}
