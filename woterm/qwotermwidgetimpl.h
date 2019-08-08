#pragma once

#include "qwowidget.h"

#include <QPointer>

class QSplitter;

class QWoTermWidgetImpl : public QWoWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidgetImpl(QString target, QWidget *parent=nullptr);
    ~QWoTermWidgetImpl();
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void onRootSplitterDestroy();

private:
    void broadcastEvent(QEvent *e);
private:
    QString m_target;
    QPointer<QSplitter> m_root;
};
