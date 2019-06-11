#pragma once

#include <QObject>

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
