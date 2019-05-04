#pragma once

#include <qtermwidget.h>

class QProcess;

class QWoTermWidget : public QTermWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidget(QWidget *parent=nullptr);

private
slots:
    void onTimeout();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinish(int code);

private:
    QProcess *m_pProcess;
};
