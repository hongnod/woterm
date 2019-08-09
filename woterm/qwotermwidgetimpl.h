#pragma once

#include "qwowidget.h"

#include <QPointer>

class QSplitter;
class QWoTermWidget;

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
    void broadcastMessage(int type, QVariant msg);
    void addToList(QWoTermWidget *w);
    void removeFromList(QWoTermWidget *w);
private:
    friend class QWoTermWidget;
    QString m_target;
    QPointer<QSplitter> m_root;
    QList<QPointer<QWoTermWidget>> m_terms;
};
