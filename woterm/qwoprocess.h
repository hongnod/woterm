#pragma once

#include <QProcess>
#include <QPointer>

class QTermWidget;
class QWoTermWidget;
class QMenu;

class QWoProcess : public QProcess
{
    Q_OBJECT
public:
    explicit QWoProcess(QObject *parent=nullptr);
protected:
    virtual void setTermWidget(QTermWidget *widget);
    virtual void prepareContextMenu(QMenu *menu);
protected:
    QPointer<QTermWidget> m_term;
    friend class QWoTermWidget;
};
