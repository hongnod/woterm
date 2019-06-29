#include <QObject>
#include <QPointer>
#include <QLocalSocket>
#include <QByteArray>

#include "qwoipc.h"

class QWoSocket : public QObject
{
    Q_OBJECT
public:
    explicit QWoSocket(FunIpcCallBack cb, QObject *parent=nullptr);
    virtual ~QWoSocket();

    void connect(const QString& name);
    bool send(const QStringList& funArgs);

private:
signals:
    void start(const QString& name);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QLocalSocket::LocalSocketError socketError);
    void onReadyRead();
    void onStart(const QString& name);

private:
    QPointer<QLocalSocket> m_socket;
    FunIpcCallBack m_cb;
};

bool qSendTo(QLocalSocket *socket, const QStringList &funArgs);
QStringList qRecvFrom(QLocalSocket *socket);
