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

enum PlaybackStopReason {
    Overridden = 0,
    None = 1,
    Default = 2,
    StopButton = 3
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
    bool Connect(QString name);
    void Disconnect(QString name);
    void DisconnectAll();
    void StartPlayback();
    void StopPlayback(int reason = PlaybackStopReason::None);
    void TrackFinishedCallback();
    void Pause();
    void Play();

    std::vector<AudioDevice> Devices;
    std::vector<AudioEngine> Engines;

    ISound *CurrentTrack = nullptr;

signals:
    void TrackFinished(int reason);
    void TrackStarted(TrackInfo *track);

public slots:

private:
    SoundReceiver *_receiver;
    int _stopReason = PlaybackStopReason::None;

    AudioDevice *_getAudioDeviceByName(QString name);
    AudioEngine *_getAudioEngineByName(QString name);

};

#endif // AUDIOWRAPPER_H
