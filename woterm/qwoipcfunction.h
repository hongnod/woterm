#pragma once

#include <QObject>
#include <QPointer>

class QLocalSocket;
class QWoIpcFunction : public QObject
{
    Q_OBJECT
public:
    explicit QWoIpcFunction(QLocalSocket *socket, QObject *parent=nullptr);
    virtual ~QWoIpcFunction();
private:
    QPointer<QLocalSocket> m_socket;
};
