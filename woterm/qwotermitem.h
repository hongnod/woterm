#pragma once

#include "qwoitem.h"
#include <QPointer>

class QWoTermWidget;
class QQuickWidget;
class QWoSshProcess;

class QWoTermItem: public QWoItem
{
    Q_OBJECT
public:
    explicit QWoTermItem(QQuickItem *parent = nullptr);
    virtual ~QWoTermItem();


    static QQuickWidget *container();

    Q_INVOKABLE void connect(const QString& remote);
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

private slots:
    void onVisibleChanged();
private:
    virtual bool event(QEvent *ev);

private:
    void syncTermGeometry();

private:
    QPointer<QWoTermWidget> m_term;
    QPointer<QWoSshProcess> m_sshRemote;
};
