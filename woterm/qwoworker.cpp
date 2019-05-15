#include "qwoworker.h"

#include <QThread>

QWoWorker::QWoWorker(QObject *parent)
    : QObject (parent)
    , m_thread(new QThread())
{
    moveToThread(m_thread);
}

QWoWorker::~QWoWorker()
{
    delete m_thread;
}
