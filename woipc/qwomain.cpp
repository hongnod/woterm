#include "qwomain.h"
#include "qwosocket.h"

#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

QWoMain::QWoMain(QObject *parent)
    : QThread (parent)
{

}

QWoMain::~QWoMain()
{

}

void QWoMain::run()
{
    qDebug() << "QWoThread";
    char*** argv = __p___argv();
    int* argc = __p___argc();
    QCoreApplication app(*argc, *argv);

    QWoSocket socket;
    socket.connect("IpcServer");
    app.exec();
}
