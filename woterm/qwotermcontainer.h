#pragma once

#include "qwowidget.h"

#include <QPointer>

class QWoTermContainer : public QWoWidget
{
    Q_OBJECT
public:
    explicit QWoTermContainer(QWidget *parent=nullptr);
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
};
