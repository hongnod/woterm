#pragma once

#include <QObject>
#include <QPointer>

class QThread;

class QWoWorker : public QObject
{
    Q_OBJECT
public:
    explicit QWoWorker(QObject *parent=nullptr);
    virtual ~QWoWorker();
private:
    QPointer<QThread> m_thread;
};
