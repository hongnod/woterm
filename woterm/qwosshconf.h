#pragma once

#include <QObject>

#include <QHash>

class QWoSshConf : public QObject
{
public:
    typedef struct{
        QString name;
        QString host;
        int port;
        QString user;
        QString password;
        QString identityFile;
        QString proxyJump;
        QString comment;
    }HostInfo;
public:
    explicit QWoSshConf(QObject *parent = nullptr);
    bool load(const QString& conf);
    bool save(const QString& conf);
private:
    void copyHostInfo(HostInfo& dst, const HostInfo& src);
private:
    QList<HostInfo> m_hosts;
};
