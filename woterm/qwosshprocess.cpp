#include "qwosshprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QThread>
#include <QProcess>
#include <QFileDialog>
#include <QClipboard>
#include <QLocalServer>
#include <QLocalSocket>

QWoSshProcess::QWoSshProcess()
    : QWoProcess (nullptr)
    , m_exeSend("D:\\woterm\\lszrz\\sbin\\x64\\Debug\\sz.exe")
    , m_exeRecv("d:/woterm/rz.exe")
{
    setProgram("D:\\woterm\\openssh\\win32\\sbin\\x64\\Debug\\ssh.exe");
    QStringList args;
    args << "-F" << "D:\\config" << "target";
    setArguments(args);

    QString name = QString("WoTerm%1_%2").arg(QApplication::applicationPid()).arg(quint64(this));
    m_server = new QLocalServer(this);
    m_server->listen(name);
    QStringList env = environment();
    env << "TERM_MSG_CHANNEL="+name;
   // env << "ProgramData="+QApplication::applicationDirPath();
    setEnvironment(env);

    QObject::connect(m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

QWoSshProcess::~QWoSshProcess()
{
}

void QWoSshProcess::zmodemSend(const QStringList &files)
{

}

void QWoSshProcess::zmodemRecv()
{

}

void QWoSshProcess::onNewConnection()
{
    QLocalSocket* local = m_server->nextPendingConnection();
    local->setTextModeEnabled(true);
    QObject::connect(local, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    QObject::connect(local, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    QObject::connect(local, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
}

void QWoSshProcess::onClientError(QLocalSocket::LocalSocketError socketError)
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    if(local) {
        local->deleteLater();
    }
}

void QWoSshProcess::onClientDisconnected()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    if(local) {
        local->deleteLater();
    }
}

void QWoSshProcess::onClientReadyRead()
{
    QLocalSocket *local = qobject_cast<QLocalSocket*>(sender());
    char buf[512];
    buf[3] = '\0';
    local->read(buf, 3);
    int len = QString(buf).toInt();
    buf[len] = '\0';
    local->read(buf, len);
    QString data(buf);
    if(data.startsWith("isread")) {
        local->setObjectName("reader");
        m_reader = local;
    }else if (data.startsWith("iswrite")) {
        local->setObjectName("writer");
        m_writer = local;
        updateTermSize();
    }else if (data.startsWith("getwinsize")) {
        updateTermSize();
    }
}

void QWoSshProcess::onZmodemSend()
{
    if(m_zmodem) {
        return;
    }
    if(m_fileDialog == nullptr) {
        m_fileDialog = new QFileDialog(m_term);
        m_fileDialog->setFileMode(QFileDialog::ExistingFiles);
        QObject::connect(m_fileDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(onFileDialogFilesSelected(const QStringList&)));
    }
    m_fileDialog->open();
}

void QWoSshProcess::onZmodemRecv()
{
    if(m_zmodem) {
        return;
    }
}

void QWoSshProcess::onZmodemAbort()
{
    if(m_zmodem == nullptr) {
        return;
    }
}

void QWoSshProcess::onFileDialogFilesSelected(const QStringList &files)
{
    QStringList args;
    for(int i = 0; i < files.length(); i++) {
        QString path = files.at(i);
        args.push_back(path);
    }
    m_zmodem = new QWoProcess(this);
    m_zmodem->enableDebugConsole(false);
    QObject::connect(m_zmodem, SIGNAL(readyReadStandardOutput()), this, SLOT(onZmodemReadyReadStandardOutput()));
    QObject::connect(m_zmodem, SIGNAL(readyReadStandardError()), this, SLOT(onZmodemReadyReadStandardError()));
    QObject::connect(m_zmodem, SIGNAL(finished(int)), this, SLOT(onZmodemFinished(int)));
    m_zmodem->setProgram(m_exeSend);
    m_zmodem->setArguments(args);
    m_zmodem->start();
}

void QWoSshProcess::onZmodemFinished(int code)
{
    Q_UNUSED(code);
    m_zmodem->deleteLater();
    write("\n");
}

void QWoSshProcess::onZmodemReadyReadStandardOutput()
{
    if(m_zmodem) {
        static int count = 0;
        QByteArray read = m_zmodem->readAllStandardOutput();
        qDebug() << "zrecv" << count++ << read.length();
        write(read);
    }
}

void QWoSshProcess::onZmodemReadyReadStandardError()
{
    if(m_zmodem) {
        QByteArray out = m_zmodem->readAllStandardError();
        m_term->parseSequenceText(out);
    }
}

void QWoSshProcess::updateTermSize()
{
    if(m_writer == nullptr) {
        return;
    }
    int linecnt = m_term->screenLinesCount();
    int column = m_term->screenColumnsCount();
    QString fun = QString("setwinsize(%1,%2)").arg(column).arg(linecnt);
    QByteArray cmd = QString("%1%2").arg(fun.size(), 3, 10, QChar('0')).arg(fun).toUtf8();
    //qDebug() << "length:" << cmd.length() << "cmd:" << cmd.data();
    m_writer->write(cmd);
}

bool QWoSshProcess::readStandardOutputFilter()
{
    if(m_zmodem) {
        QByteArray data = readAllStandardOutput();
        m_zmodem->write(data);
        return true;
    }
    return false;
}

bool QWoSshProcess::readStandardErrorFilter()
{
    if(m_zmodem) {
        QByteArray data = readAllStandardError();
        m_zmodem->writeError(data);
        return true;
    }
    return false;
}

bool QWoSshProcess::finishFilter(int code)
{
    return false;
}

bool QWoSshProcess::writeFilter(const QByteArray &data)
{
    return false;
}

bool QWoSshProcess::eventFilter(QObject *obj, QEvent *ev)
{
    QEvent::Type t = ev->type();
    if (t == QEvent::Resize) {
        QMetaObject::invokeMethod(this, "updateTermSize",Qt::QueuedConnection);
    }
    return false;
}

void QWoSshProcess::setTermWidget(QTermWidget *widget)
{
    QWoProcess::setTermWidget(widget);
    widget->installEventFilter(this);
}

void QWoSshProcess::prepareContextMenu(QMenu *menu)
{
    if(m_zmodemSend == nullptr) {
        m_zmodemSend = menu->addAction(tr("Zmodem Upload"));
        m_zmodemRecv = menu->addAction(tr("Zmodem Receive"));
        m_zmodemAbort = menu->addAction(tr("Zmoddem Abort"));

        QObject::connect(m_zmodemSend, SIGNAL(triggered()), this, SLOT(onZmodemSend()));
        QObject::connect(m_zmodemRecv, SIGNAL(triggered()), this, SLOT(onZmodemRecv()));
        QObject::connect(m_zmodemAbort, SIGNAL(triggered()), this, SLOT(onZmodemAbort()));
    }
}
