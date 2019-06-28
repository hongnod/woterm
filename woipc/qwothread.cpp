#include "qwothread.h"

#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>

QWoThread::QWoThread(QObject *parent)
    : QThread (parent)
{

}

QWoThread::~QWoThread()
{

}

void QWoThread::run()
{
    qDebug() << "QWoThread";
    QEventLoop loop;

    loop.exec();
}
