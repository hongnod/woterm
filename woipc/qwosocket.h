#include <QObject>
#include <QPointer>
#include <QLocalSocket>
#include <QByteArray>

class QWoSocket : public QObject
{
    Q_OBJECT
public:
    explicit QWoSocket(QObject *parent=nullptr);
    virtual ~QWoSocket();

    void connect(const QString& name);
    bool send(const QStringList& funArgs);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QLocalSocket::LocalSocketError socketError);
    void onReadyRead();

private:
    QPointer<QLocalSocket> m_socket;
};

bool qSendTo(QLocalSocket *socket, const QStringList &funArgs);
QStringList qRecvFrom(QLocalSocket *socket);
