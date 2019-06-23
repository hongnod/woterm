#pragma once

#include <QObject>
#include <QPointer>

class QLocalSocket;
class QWoLocalSocket : public QObject
{
    Q_OBJECT
public:
    QWoLocalSocket(QLocalSocket *socket, QObject *parent=nullptr);
    virtual ~QWoLocalSocket();
private:
    QPointer<QLocalSocket> m_socket;
};
