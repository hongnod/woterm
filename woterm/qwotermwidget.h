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
private
slots:
    void onTimeout();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished(int code);
    void onSendData(const QByteArray& buf);
    void onCopyToClipboard();
    void onPasteFromClipboard();

private:
    void contextMenuEvent(QContextMenuEvent *event);

private:
    QPointer<QWoProcess> m_process;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_copy;
    QPointer<QAction> m_paste;
};
