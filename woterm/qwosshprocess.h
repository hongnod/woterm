#pragma once

#include "qwoprocess.h"

#include <QPointer>
#include <QLocalSocket>

class QTermWidget;
class QLocalServer;
class QLocalSocket;
class QAction;
class QFileDialog;
class QThread;

class QWoSshProcess : public QWoProcess
{
    Q_OBJECT
public:
    explicit QWoSshProcess(const QString& target);
    virtual ~QWoSshProcess();

private slots:
    void onNewConnection();
    void onClientError(QLocalSocket::LocalSocketError socketError);
    void onClientDisconnected();
    void onClientReadyRead();
    void onZmodemSend();
    void onZmodemRecv();
    void onZmodemAbort();
    void onZmodemFinished(int code);
    void onZmodemReadyReadStandardOutput();
    void onZmodemReadyReadStandardError();
    void onFileDialogFilesSelected(const QStringList& files);
    void onTermTitleChanged();
    void onDuplicateSession();

private:
    Q_INVOKABLE void updateTermSize();
private:
    virtual bool eventFilter(QObject *obj, QEvent *ev);
    virtual void setTermWidget(QTermWidget *widget);
    virtual void prepareContextMenu(QMenu *menu);

private:
    void zmodemSend(const QStringList& files);
    void zmodemRecv();
    QWoProcess *createZmodem();
    bool isRzCommand(const QByteArray& data);
private:
    QPointer<QLocalServer> m_server;
    QPointer<QLocalSocket> m_reader;
    QPointer<QLocalSocket> m_writer;
    QPointer<QAction> m_zmodemDupl;
    QPointer<QAction> m_zmodemSend;
    QPointer<QAction> m_zmodemRecv;
    QPointer<QAction> m_zmodemAbort;
    QPointer<QFileDialog> m_fileDialog;

    QPointer<QWoProcess> m_zmodem;
    QString m_exeSend;
    QString m_exeRecv;
    QString m_title;
};
