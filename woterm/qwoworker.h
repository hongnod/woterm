#pragma once

#include <QObject>
#include <QPointer>

class QThread;

class QWoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QWoWorker(QObject *parent=nullptr);
private:
    QPointer<QThread> m_thread;
};
