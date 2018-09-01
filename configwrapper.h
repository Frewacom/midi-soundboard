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
#include "helpers.h"

struct AudioDevicePacket {
    QString Name;
    int Volume;
};

struct ProfilePacket {
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
    void SaveBinding(int key, QString action);
    void SaveProfile(ProfilePacket *packet);
    void SaveCurrentProfile(QString name);

signals:

public slots:

private:
    QDir _applicationFolder;
    QString _settingsPath;
    QJsonDocument _settingsContents;
    QJsonObject _settingsJSON;

    QJsonObject _currentProfile;
    ProfilePacket *_currentProfilePacket = nullptr;

    void _parseSettingsJSON();
    ProfilePacket* _packetProfileJSON(QJsonObject profile);

};

#endif // CONFIGWRAPPER_H
