#pragma once

#include "qwoshowerwidget.h"

#include <QPointer>

class QSplitter;
class QWoTermWidget;
class QWoProcess;

class QWoShellWidgetImpl : public QWoShowerWidget
{
    Q_OBJECT
public:
    explicit QWoShellWidgetImpl(QWidget *parent=nullptr);
    ~QWoShellWidgetImpl();

signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onRootSplitterDestroy();

private:
    friend class QWoTermWidget;
    const QString m_target;
    QPointer<QSplitter> m_root;
    QList<QPointer<QWoTermWidget>> m_terms;
};
