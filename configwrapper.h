#ifndef CONFIGWRAPPER_H
#define CONFIGWRAPPER_H

#include <QObject>
#include <QDir>
#include <QList>
#include <QMap>
#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDirIterator>
#include "helpers.h"

struct AudioDevicePacket {
    QString Name;
    int Volume;
};

struct ProfilePacket {
    QString FileName;
    QString Name;
    QString MidiDevice;
    QList<AudioDevicePacket*> AudioDevices;
    QMap<int, QString> Bindings;
};

class ConfigWrapper : public QObject
{
    Q_OBJECT
public:
    explicit ConfigWrapper(QObject *parent = nullptr);
    ~ConfigWrapper();

    ProfilePacket* GetCurrentProfile();
    void UpdateDeviceVolume(QString name, int volume);
    QString GetBinding(int key);
    int GetProfiles();
    ProfilePacket* GetProfileByName(QString name);
    void SetCurrentProfile(ProfilePacket *packet);
    void SaveBinding(int key, QString action);
    void SaveProfile();
    void SaveMidiDevice(QString device);

signals:
    void on_Profile_found(QString name);

public slots:

private:
    QDir _applicationFolder;
    QString _settingsPath;
    QJsonDocument _settingsContents;
    QJsonObject _settingsJSON;

    QJsonObject _currentProfile;
    ProfilePacket *_currentProfilePacket = nullptr;
    QMap<QString, ProfilePacket*> _profilePackets;

    void _parseSettingsJSON();
    ProfilePacket* _packetProfileJSON(QString fileName, QJsonObject profile);

};

#endif // CONFIGWRAPPER_H
