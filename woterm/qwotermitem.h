#pragma once

#include "qwoitem.h"
#include <QPointer>

class QWoTermWidget;

class QWoTermItem: public QWoItem
{
    Q_OBJECT
public:
    explicit QWoTermItem(QQuickItem *parent = nullptr);
    virtual ~QWoTermItem();
private:
    QPointer<QWoTermWidget> m_term;
};
