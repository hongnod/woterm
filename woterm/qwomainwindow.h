#pragma once

#include <QMainWindow>
#include <QPointer>

class QWoMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QWoMainWindow(QWidget *parent=nullptr);
protected:
    void closeEvent(QCloseEvent *event);
};
