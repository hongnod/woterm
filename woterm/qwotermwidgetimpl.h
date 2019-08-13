#pragma once

#include "qwoshowerwidget.h"

#include <QPointer>

class QSplitter;
class QWoTermWidget;
class QWoProcess;

class QWoTermWidgetImpl : public QWoShowerWidget
{
    Q_OBJECT
public:
    typedef enum TermType {
        EShell = 0x1,
        ESsh = 0x2
    } ETermType;
public:
    explicit QWoTermWidgetImpl(QWidget *parent=nullptr);
    explicit QWoTermWidgetImpl(QString target, QWidget *parent=nullptr);
    ~QWoTermWidgetImpl();

    ETermType termType();
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
    void init(QWoProcess *process);
private:
    friend class QWoTermWidget;
    const QString m_target;
    QPointer<QSplitter> m_root;
    QList<QPointer<QWoTermWidget>> m_terms;
    const ETermType m_termType;
};
