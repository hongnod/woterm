#pragma once

#include "qwoipc.h"

#include <QMap>
#include <QPointer>
#include <QWaitCondition>

class QWoSocket;

class QWoMain : public QObject
{
    Q_OBJECT
public:
    explicit QWoMain(QObject *parent=nullptr);
    virtual ~QWoMain();

    static QWoMain *instance();

    void init();

    int connect(const QString& name, FunIpcCallBack cb);
    void send(int id, const QStringList& funArgs);
    void close(int id);
private:
    QMap<int, QPointer<QWoSocket>> m_locals;
    QWaitCondition m_cond;
};


