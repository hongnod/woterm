#include "qwosshprocess.h"
#include "qwoevent.h"
#include "qwosetting.h"

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
#include <QMessageBox>

bool qWrite(QLocalSocket *socket, char* data, int len) {
    int nleft = len;
    char *buf = data;
    while(nleft > 0) {
        int n = socket->write(buf, nleft);
        if(n < 0) {
            return false;
        }
        nleft -= n;
        buf += n;
    }
    return true;
}

bool qRead(QLocalSocket *socket, char* data, int len) {
    int nleft = len;
    char *buf = data;
    while(nleft > 0) {
        int n = socket->read(buf, nleft);
        if(n < 0) {
            return false;
        }
        if(n == 0) {
            if(!socket->waitForReadyRead()) {
                return false;
            }
        }
        nleft -= n;
        buf += n;
    }
    return true;
}

bool qSendTo(QLocalSocket *socket, const QStringList &funArgs)
{
    QByteArray buf;
    QDataStream in(&buf, QIODevice::WriteOnly);
    in << funArgs;
    int length = buf.length();
    if(!qWrite(socket, (char*)&length, sizeof(int))){
        return false;
    }
    return qWrite(socket, buf.data(), length);
}

QStringList qRecvFrom(QLocalSocket *socket)
{
    QByteArray buf;
    int length;
    if(socket->bytesAvailable() <= 0) {
        return QStringList();
    }
    if(!qRead(socket, (char*)&length, sizeof(int))) {
        return QStringList();
    }
    buf.resize(length);
    if(!qRead(socket, buf.data(), length)) {
        return QStringList();
    }
    QStringList funArgs;
    QDataStream out(buf);
    out >> funArgs;
    return funArgs;
}

QWoSshProcess::QWoSshProcess(const QString& target, QObject *parent)
    : QWoProcess (parent)
{
    m_exeSend = QWoSetting::zmodemSZPath();
    if(m_exeSend.isEmpty()){
        QMessageBox::warning(m_term, "zmodem", "can't find sz program.");
    }
    m_exeRecv = QWoSetting::zmodemRZPath();
    if(m_exeRecv.isEmpty()) {
        QMessageBox::warning(m_term, "zmodem", "can't find rz program.");
    }
    QString program = QWoSetting::sshProgramPath();
    if(program.isEmpty()) {
        QMessageBox::critical(m_term, "ssh", "can't find ssh program.");
        QApplication::exit(0);
        return;
    }
    QString cfg = QWoSetting::sshServerListPath();
    if(cfg.isEmpty()) {
        QMessageBox::critical(m_term, "ssh server list", "no path for ssh server list.");
        QApplication::exit(0);
        return;
    }
    QString ipc = QWoSetting::ipcProgramPath();
    if(ipc.isEmpty()) {
        QMessageBox::critical(m_term, "ipc", "can't find ipc program.");
        QApplication::exit(0);
        return;
    }
    m_target = target;
    setProgram(program);
    QStringList args;
    args.append(target);
    args.append("-F");
    args.append(cfg);
    setArguments(args);

    QString name = QString("WoTerm%1_%2").arg(QApplication::applicationPid()).arg(quint64(this));
    m_server = new QLocalServer(this);
    m_server->listen(name);
    QStringList env = environment();
    env << "TERM_MSG_IPC_NAME="+name;
    env << "TERM_MSG_IPC_PROGRAM="+ipc;
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

QWoProcess *QWoSshProcess::createZmodem()
{
    QWoProcess *process = new QWoProcess(this);
    process->enableDebugConsole(false);
    QObject::connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(onZmodemReadyReadStandardOutput()));
    QObject::connect(process, SIGNAL(readyReadStandardError()), this, SLOT(onZmodemReadyReadStandardError()));
    QObject::connect(process, SIGNAL(finished(int)), this, SLOT(onZmodemFinished(int)));
    return process;
}

bool QWoSshProcess::isRzCommand(const QByteArray &ba)
{
//    char rzhdr[6] = { 0 };
//        rzhdr[0] = '*';
//        rzhdr[1] = '*';
//        rzhdr[2] = 30;
//        rzhdr[3] = 'B';
    //check \r\nrz\r
    const char *buf = ba.data();
    int len = ba.length();
    if (len >= 3 && len < 30) {
        for (int i = 0; i < 10; i++) {
            if (buf[i] == 'r' && buf[i + 1] == 'z' && buf[i + 2] == '\r') {
                if (i > 0 && (buf[i - 1] == '\r' || buf[i - 1] == '\n')) {
                    return true;
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}

void QWoSshProcess::onNewConnection()
{
    m_ipc = m_server->nextPendingConnection();
    m_reader = new FunArgReader(m_ipc, this);
    m_writer = new FunArgWriter(m_ipc, this);
    QObject::connect(m_ipc, SIGNAL(disconnected()), this, SLOT(onClientDisconnected()));
    QObject::connect(m_ipc, SIGNAL(error(QLocalSocket::LocalSocketError)), this, SLOT(onClientError(QLocalSocket::LocalSocketError)));
    QObject::connect(m_ipc, SIGNAL(readyRead()), this, SLOT(onClientReadyRead()));
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
    m_reader->readAll();
    while(1) {
        QStringList funArgs = m_reader->next();
        if(funArgs.length() <= 0) {
            return;
        }
        if(funArgs[0] == "ping") {

        }else if(funArgs[0] == "getwinsize") {
            updateTermSize();
        }
    }
}

void QWoSshProcess::onZmodemSend()
{
    if(m_zmodem) {
        return;
    }
    if(m_fileDialog == nullptr) {
        m_fileDialog = new QFileDialog(m_term, tr("FileSelect"));
        m_fileDialog->setFileMode(QFileDialog::ExistingFiles);
        QObject::connect(m_fileDialog, SIGNAL(filesSelected(const QStringList&)), this, SLOT(onFileDialogFilesSelected(const QStringList&)));
    }
    QString path = QWoSetting::value("zmodem/lastPath").toString();
    m_fileDialog->setDirectory(path);
    m_fileDialog->open();
}

void QWoSshProcess::onZmodemRecv()
{
    if(m_zmodem) {
        return;
    }
    m_zmodem = createZmodem();
    m_zmodem->setProgram(m_exeRecv);
    QStringList args;
    args << "rz";
    m_zmodem->setArguments(args);
    m_zmodem->setWorkingDirectory("d:\\zmodem");
    m_zmodem->start();
}

void QWoSshProcess::onZmodemAbort()
{
    static char canistr[] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0};
    if(m_zmodem == nullptr) {
        return;
    }
    if(m_zmodem->program() == m_exeRecv) {
        write(canistr);
    }else{
        m_zmodem->close();
        write(canistr);
    }
}

void QWoSshProcess::onFileDialogFilesSelected(const QStringList &files)
{
    QStringList args;
    QString path = files.front();
    int idx = path.lastIndexOf('/');
    if(idx > 0) {
        path = path.left(idx);
        QWoSetting::setValue("zmodem/lastPath", path);
    }
    for(int i = 0; i < files.length(); i++) {
        QString path = files.at(i);
        args.push_back(path);
    }
    m_zmodem = createZmodem();
    m_zmodem->setProgram(m_exeSend);
    m_zmodem->setArguments(args);
    m_zmodem->start();
}

void QWoSshProcess::onTermTitleChanged()
{
    QString name = m_term->title();
}

void QWoSshProcess::onDuplicateSession()
{
    QString path = QApplication::applicationFilePath();
    path.append(" ");
    path.append(m_target);
    QProcess::startDetached(path);
}

void QWoSshProcess::onZmodemFinished(int code)
{
    Q_UNUSED(code);
    m_zmodem->deleteLater();
    write("\r");
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
    int linecnt = m_term->screenLinesCount();
    int column = m_term->screenColumnsCount();
    QStringList funArgs;
    funArgs << "setwinsize" << QString("%1").arg(column) << QString("%1").arg(linecnt);
    if(m_ipc) {
        qDebug() << funArgs;
        m_writer->write(funArgs);
    }
}

bool QWoSshProcess::eventFilter(QObject *obj, QEvent *ev)
{
    QEvent::Type t = ev->type();
    if (t == QEvent::Resize) {
        QMetaObject::invokeMethod(this, "updateTermSize",Qt::QueuedConnection);
    }else if(t == QWoEvent::EventType) {
        QWoEvent *we = (QWoEvent*)ev;
        QWoEvent::WoEventType t = we->eventType();
        if(t == QWoEvent::BeforeReadStdOut) {
            if(m_zmodem) {
                QByteArray data = readAllStandardOutput();
                m_zmodem->write(data);
                we->setAccepted(true);
            }else{

                we->setAccepted(false);
            }
        }else if(t == QWoEvent::BeforeReadStdErr) {

        }else if(t == QWoEvent::AfterReadStdOut) {
            QByteArray data = we->data().toByteArray();
            if(isRzCommand(data)) {
                onZmodemRecv();
            }
        }else if(t == QWoEvent::AfterReadStdErr) {
            if(m_zmodem) {
                QByteArray data = readAllStandardError();
                m_zmodem->writeError(data);
                we->setAccepted(true);
            }else{
                we->setAccepted(false);
            }
        }else if(t == QWoEvent::BeforeFinish) {

        }

        return true;
    }
    return false;
}

void QWoSshProcess::setTermWidget(QTermWidget *widget)
{
    QWoProcess::setTermWidget(widget);
    widget->installEventFilter(this);
    QObject::connect(m_term, SIGNAL(titleChanged()), this, SLOT(onTermTitleChanged()));
    QWidget *topLevel = m_term->topLevelWidget();
    topLevel->setWindowTitle(m_target);
}

void QWoSshProcess::prepareContextMenu(QMenu *menu)
{
    if(m_zmodemSend == nullptr) {
        menu->addAction(tr("Find..."), m_term, SLOT(toggleShowSearchBar()), QKeySequence(Qt::CTRL +  Qt::Key_F));
        m_zmodemDupl = menu->addAction(tr("Duplicate Session"), this, SLOT(onDuplicateSession()));
        m_zmodemSend = menu->addAction(tr("Zmodem Upload"), this, SLOT(onZmodemSend()));
        m_zmodemRecv = menu->addAction(tr("Zmodem Receive"), this, SLOT(onZmodemRecv()));
        m_zmodemAbort = menu->addAction(tr("Zmoddem Abort"), this, SLOT(onZmodemAbort()), QKeySequence(Qt::CTRL +  Qt::Key_C));
    }
}
