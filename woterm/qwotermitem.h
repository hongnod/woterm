#pragma once

#include "qwoitem.h"

class QWoTermItem: public QWoItem
{
    Q_OBJECT
public:
    explicit QWoTermItem(QQuickItem *parent = nullptr);
    virtual ~QWoTermItem();
};
