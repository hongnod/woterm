#pragma once

#include <QObject>
#include <QPointer>

class QLocalServer;
class QWoLocalServer : public QObject
{
    Q_OBJECT
public:
    QWoLocalServer(QObject *parent=nullptr);
    virtual ~QWoLocalServer();
private:
    QPointer<QLocalServer> m_server;
};
