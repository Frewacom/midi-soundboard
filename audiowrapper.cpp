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

bool AudioWrapper::Connect(AudioDevice *device) {
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
    }

    return false;
}

void AudioWrapper::StartPlayback() {
    QDir path = Helpers::GetApplicationPath();

    for (int i = 0; i < this->Engines.size(); i++) {
        // TODO: Make sure we only assign CurrentTrack once
        this->CurrentTrack = this->Engines.at(i).Engine->play2D(
            path.filePath("music/my-best.mp3").toStdString().c_str(),
            false,
            false,
            true
        );
    }

    this->CurrentTrack->setSoundStopEventReceiver(this->_receiver);

    // Track name should be extracted from the path
    TrackInfo *track = new TrackInfo();
    track->Name ="my-best.mp3";
    track->Length = this->CurrentTrack->getPlayLength();
    emit this->TrackStarted(track);
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

void AudioWrapper::TrackFinishedCallback() {
    this->CurrentTrack = nullptr;

    emit this->TrackFinished();
}

AudioWrapper::~AudioWrapper() {
    for (int i = 0; i < this->Engines.size(); i++) {
        this->Engines.at(i).Engine->drop();
    }

    this->Engines.clear();
    this->Devices.clear();
}
