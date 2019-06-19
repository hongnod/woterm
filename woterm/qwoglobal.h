#pragma once

#include <QObject>
#include <QUuid>

typedef struct{
    QString name;
    QString host;
    int port;
    QString user;
    QString password;
    QString identityFile;
    QString proxyJump;
    QString memo;
}HostInfo;

Q_DECLARE_METATYPE(HostInfo)


