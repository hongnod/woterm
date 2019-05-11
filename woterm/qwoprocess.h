#pragma once

#include <QProcess>
#include <QPointer>

class QTermWidget;

class QWoProcess : public QProcess
{
    Q_OBJECT
public:
    explicit QWoProcess(QObject *parent=nullptr);
    void setTermWidget(QTermWidget *widget);
protected:
    QPointer<QTermWidget> m_term;
};
