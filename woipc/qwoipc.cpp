#include "qwoipc.h"
#include "qwomain.h"

#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include <QMutex>
#include <QDebug>

static QWoApp woApp;
static QWoMain woMain;

int IpcConnect(const char *name, FunIpcCallBack cb)
{
    return woMain.connect(name, cb);
}

bool IpcCall(int hdl, char *argv[], int argc)
{
    QStringList args;
    for(int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }
    woMain.send(hdl, args);
    return true;
}

bool IpcClose(int hdl)
{
    woMain.close(hdl);
    return false;
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    return;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
      fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtInfoMsg:
      fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtWarningMsg:
      fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtCriticalMsg:
      fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
    case QtFatalMsg:
      fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
      break;
    }
}

int IpcInit()
{
    qInstallMessageHandler(myMessageOutput);
    if(woApp.isRunning()) {
        return 0;
    }
    woApp.start();
    return 1;
}

void QWoApp::start()
{
    QMutex mtx;
    mtx.lock();
    QThread::start();
    m_cond.wait(&mtx, 3000);
    mtx.unlock();
}

static QCoreApplication *myApp = nullptr;
void QWoApp::run()
{
#if defined(Q_OS_WIN)
    char*** argv = __p___argv();
    int* argc = __p___argc();
#else

#endif
    myApp = new QCoreApplication(*argc, *argv);
    qDebug() << "start app thread";
    m_cond.wakeOne();
    myApp->exec();
}

void IpcExit()
{
    QCoreApplication::quit();
    woApp.wait();
    woApp.exit(0);
    woApp.terminate();

}
