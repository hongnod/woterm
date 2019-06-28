#pragma once

#include <QThread>

class QWoMain : public QThread
{
    Q_OBJECT
public:
    explicit QWoMain(QObject *parent=nullptr);
    virtual ~QWoMain();
protected:
    virtual void run();
};
