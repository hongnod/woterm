#pragma once

#include "qwoglobal.h"

#include <QObject>
#include <QHash>

class QWoSshConf : public QObject
{
public:
    explicit QWoSshConf(const QString& conf, QObject *parent = nullptr);
    static QWoSshConf* instance();
    bool load();
    bool save();
    bool exportTo(const QString& path);
    void remove(const QString& name);
    void append(const HostInfo& hi);

    QList<HostInfo> hostList() const;
    QStringList hostNameList() const;

    int findHost(const QString& name);
    HostInfo hostInfo(int i);
private:
    QString m_conf;
    QList<HostInfo> m_hosts;
};
