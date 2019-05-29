#pragma once

#include "qwoitem.h"
#include <QPointer>

class QWoTermWidget;
class QQuickWidget;
class QWoSshProcess;

class QWoTermItem: public QWoItem
{
    Q_OBJECT
public:
    explicit QWoTermItem(QQuickItem *parent = nullptr);
    virtual ~QWoTermItem();


    static QQuickWidget *container();

    Q_INVOKABLE void connect(const QString& args);
private:
    QPointer<QWoTermWidget> m_term;
    QPointer<QWoSshProcess> m_sshRemote;
};
