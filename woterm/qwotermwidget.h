#pragma once

#include <qtermwidget.h>

#include <QPointer>

class QWoProcess;
class QMenu;

class QWoTermWidget : public QTermWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidget(QWoProcess *process, QWidget *parent=nullptr);
    virtual ~QWoTermWidget();

    QWoProcess *process();
signals:
    void aboutToClose(QCloseEvent* event);
private
slots:
    void onTimeout();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished(int code);
    void onSendData(const QByteArray& buf);
    void onCopyToClipboard();
    void onPasteFromClipboard();
    void onVerticalSplitView();
    void onHorizontalSplitView();
    void onCloseThisSession();

private:
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);
private:
    void initDefault();
    void initCustom();
    void resetProperty(QVariantMap data);
    void splitWidget(int sz, bool vertical);
private:
    QPointer<QWoProcess> m_process;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_copy;
    QPointer<QAction> m_paste;
};
