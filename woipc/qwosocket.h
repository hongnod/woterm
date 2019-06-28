#include <QObject>
#include <QPointer>
#include <QLocalSocket>

class QWoSocket : public QObject
{
    Q_OBJECT
public:
    explicit QWoSocket(QObject *parent=nullptr);
    virtual ~QWoSocket();

    void connect(const QString& name);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QLocalSocket::LocalSocketError socketError);
    void onReadyRead();

private:
    QPointer<QLocalSocket> m_socket;
};
