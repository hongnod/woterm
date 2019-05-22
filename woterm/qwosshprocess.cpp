#include "qwosshprocess.h"
#include "qwoevent.h"

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
    , m_exeRecv("D:\\woterm\\lszrz\\sbin\\x64\\Debug\\rz.exe")
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
    for(int i = 0; i < files.length(); i++) {
        QString path = files.at(i);
        args.push_back(path);
    }
    m_zmodem = createZmodem();
    m_zmodem->setProgram(m_exeSend);
    m_zmodem->setArguments(args);
    m_zmodem->start();
}

void QWoSshProcess::onZmodemFinished(int code)
{
    Q_UNUSED(code);
    m_zmodem->deleteLater();
    write("\r");
}

//void Session::cancelZModem()
//{
//  _shellProcess->sendData("\030\030\030\030", 4); // Abort
//  _zmodemBusy = false;
//}
//void Session::zmodemReadStatus()
//{
//  _zmodemProc->setReadChannel( QProcess::StandardError );
//  QByteArray msg = _zmodemProc->readAll();
//  while(!msg.isEmpty())
//  {
//     int i = msg.indexOf('\015');
//     int j = msg.indexOf('\012');
//     QByteArray txt;
//     if ((i != -1) && ((j == -1) || (i < j)))
//     {
//       msg = msg.mid(i+1);
//     }
//     else if (j != -1)
//     {
//       txt = msg.left(j);
//       msg = msg.mid(j+1);
//     }
//     else
//     {
//       txt = msg;
//       msg.truncate(0);
//     }
//     if (!txt.isEmpty())
//       _zmodemProgress->addProgressText(QString::fromLocal8Bit(txt));
//  }
//}

//int isRzCommand(char *buf, int len) {
//	if (len >= 3 && len < 30) {
//		for (int i = 0; i < 10; i++) {
//			if (buf[i] == 114 && buf[i + 1] == 122 && buf[i + 2] == 13) {
//				if (i > 0 && buf[i - 1] == 10 || buf[i - 1] == 13) {
//					return isFromServer();
//				} else {
//					return isFromServer();
//				}
//			}
//		}
//	}
//	return 0;
//}



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
