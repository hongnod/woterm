#pragma once

#include "qwoshowerwidget.h"

#include <QPointer>

class QSplitter;
class QWoTermWidget;
class QWoProcess;
class QMenu;

class QWoTermWidgetImpl : public QWoShowerWidget
{
    Q_OBJECT
public:
    typedef enum TermType {
        EShell = 0x1,
        ESsh = 0x2
    } ETermType;
public:
    explicit QWoTermWidgetImpl(QString target, QWidget *parent=nullptr);
    ~QWoTermWidgetImpl();

    ETermType termType();
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void handleTabMouseEvent(QMouseEvent* ev);

private slots:
    void onRootSplitterDestroy();
    void onModifyThisSession();
    void onCloseThisSession();

private:
    void broadcastMessage(int type, QVariant msg);
    void addToList(QWoTermWidget *w);
    void removeFromList(QWoTermWidget *w);
private:
    friend class QWoTermWidget;
    const QString m_target;
    QPointer<QSplitter> m_root;
    QList<QPointer<QWoTermWidget>> m_terms;
    const ETermType m_termType;
    QPointer<QMenu> m_menu;
};
