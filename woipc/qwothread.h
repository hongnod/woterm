#pragma once

#include <QThread>

class QWoThread : public QThread
{
    Q_OBJECT
public:
    explicit QWoThread(QObject *parent=nullptr);
    virtual ~QWoThread();
protected:
    virtual void run();
};
