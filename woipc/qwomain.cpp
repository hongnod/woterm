#include "qwomain.h"
#include "qwosocket.h"

#include <QDebug>
#include <QEventLoop>
#include <QCoreApplication>
#include <QLocalSocket>
#include <QDataStream>
#include <QMutex>

#include <stdlib.h>
#include <stdio.h>

#define SOCKET_LOCALID ("localid")

QWoMain::QWoMain(QObject *parent)
    : QObject(parent)
{

}

QWoMain::~QWoMain()
{

}

int QWoMain::connect(const QString &name, FunIpcCallBack cb)
{
    QWoSocket *local = new QWoSocket(cb);
    int id = local->connect(name);
    m_locals.insert(id, local);
    return id;
}

void QWoMain::send(int id, const QStringList &funArgs)
{
    QWoSocket *local = m_locals.value(id);
    if(local) {
        local->send(funArgs);
    }
}

void QWoMain::close(int id)
{
    QWoSocket *local = m_locals.take(id);
    if(local) {
        local->close();
    }
}
