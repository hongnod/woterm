#pragma once

#include <QWidget>
#include <QPointer>

class QWoProcess;

class QWoWin32ShellWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QWoWin32ShellWidget(QWidget *parent=nullptr);
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
private:
    void init();
private:
    QPointer<QWoProcess> m_shell;
};
