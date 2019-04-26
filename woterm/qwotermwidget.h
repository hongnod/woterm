#pragma once

#include <qtermwidget.h>

class QWoTermWidget : public QTermWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidget(QWidget *parent=nullptr);
};
