#pragma once

#include <qtermwidget.h>

#include <QPointer>

class QWoProcess;
class QMenu;
class QWoTermMask;
class QWoPasswordInput;
class QWoTermWidgetImpl;

class QWoTermWidget : public QTermWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidget(QWoProcess *process, QWidget *parent=nullptr);
    virtual ~QWoTermWidget();

    QWoProcess *process();
    void closeAndDelete();

    void triggerPropertyCheck();

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
    void onForceToCloseThisSession();
    void onSessionReconnect();
    void onPasswordInputResult(const QString& pass, bool isSave);

private:
    Q_INVOKABLE void showPasswordInput(const QString& prompt, bool echo);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    void initDefault();
    void initCustom();
    void resetProperty(QVariantMap data);
    void splitWidget(int sz, bool vertical);

    QWoTermWidgetImpl *findTermImpl();
    void addToTermImpl();
    void removeFromTermImpl();

    void onBroadcastMessage(int type, QVariant msg);
private:
    friend class QWoTermWidgetImpl;
    QPointer<QWoProcess> m_process;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_copy;
    QPointer<QAction> m_paste;
    QPointer<QWoTermMask> m_mask;
    QPointer<QWoPasswordInput> m_passInput;
    bool m_bexit;
};
