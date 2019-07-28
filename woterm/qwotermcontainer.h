#pragma once

#include <QWidget>

#include <QPointer>

class QWoTermContainer : public QWidget
{
    Q_OBJECT
public:
    explicit QWoTermContainer(QWidget *parent=nullptr);
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
};
