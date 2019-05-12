#pragma once

#include <QProcess>
#include <QPointer>

class QTermWidget;
class QWoTermWidget;

class QWoProcess : public QProcess
{
    Q_OBJECT
public:
    explicit QWoProcess(QObject *parent=nullptr);
protected:
    virtual void setTermWidget(QTermWidget *widget);
protected:
    QPointer<QTermWidget> m_term;
    friend class QWoTermWidget;
};
