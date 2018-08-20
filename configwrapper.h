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
    int Id;
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

signals:
    void currentProfileLoaded(ProfilePacket *packet);

public slots:

private:
    QDir _applicationFolder;
    QString _settingsPath;
    QJsonDocument _settingsContents;
    QJsonObject _settingsJSON;
    QJsonObject _currentProfile;

    void _parseSettingsJSON();
    ProfilePacket* _packetProfileJSON(QJsonObject profile);

};

#endif // CONFIGWRAPPER_H
