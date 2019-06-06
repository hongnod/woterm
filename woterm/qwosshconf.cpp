#include "qwosshconf.h"

#include <QFile>

/*  #begin
 *  #
 *  #    <---memo--->
 *  #
 *  Host name
 *    HostName xxxx
 *    Port  22
 *    User  abc
 *    IdentityFile ~xxx
 *    Password xxxx
 *    ProxyJump xxx
 *  #end
 */

QWoSshConf::QWoSshConf(QObject *parent)
    :QObject (parent)
{

}

bool QWoSshConf::load(const QString &conf)
{
    QFile file(conf);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return false;
    }
    QList<QStringList> hosts;
    QStringList host;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        if(line == "#begin" || line == "#end" || line == "") {
            if(host.length() > 0) {
                hosts.push_back(host);
            }
            host.clear();
        } else {
            host.push_back(line);
        }
    }
    return false;
}
