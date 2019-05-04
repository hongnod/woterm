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
    void onSendData(const QByteArray& buf);

private:
    QProcess *m_pProcess;
};
