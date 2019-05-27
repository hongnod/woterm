#pragma once

#include <QQuickItem>

class QWoItem: public QQuickItem
{
    Q_OBJECT
public:
    explicit QWoItem(QQuickItem *parent = nullptr);
    virtual ~QWoItem();
};
