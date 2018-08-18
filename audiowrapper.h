#ifndef AUDIOWRAPPER_H
#define AUDIOWRAPPER_H

#include <QDir>
#include <QDebug>
#include <QObject>
#include <irrKlang.h>
#include "helpers.h"
#include "logger.h"

using namespace irrklang;

class SoundReceiver : public ISoundStopEventReceiver
{
    public:
        SoundReceiver(std::function<void()> callback)
            : ISoundStopEventReceiver()
        {
            this->Callback = callback;
        }

        std::function<void()> Callback;

        virtual void OnSoundStopped (
            irrklang::ISound* sound,
            irrklang::E_STOP_EVENT_CAUSE reason,
            void* userData
        ) {
            this->Callback();
        }
};

struct TrackInfo {
    QString Name;
    int Length;
};

struct AudioDevice {
    QString Name;
    const char* Id;
};

struct AudioEngine {
    AudioDevice *Data;
    ISoundEngine *Engine;
};

class AudioWrapper : public QObject
{
    Q_OBJECT
public:
    explicit AudioWrapper(QObject *parent = nullptr);
    ~AudioWrapper();

    void ScanForAudioDevices();
    bool Connect(AudioDevice *device);
    void StartPlayback();
    void TrackFinishedCallback();
    void Pause();
    void Play();

    std::vector<AudioDevice> Devices;
    std::vector<AudioEngine> Engines;

    ISound *CurrentTrack = nullptr;

signals:
    void TrackFinished();
    void TrackStarted(TrackInfo *track);

public slots:

private:
    SoundReceiver *_receiver;

};

#endif // AUDIOWRAPPER_H
