#pragma once

#include <qtermwidget.h>

#include <QPointer>

class QProcess;
class QMenu;

class QWoTermWidget : public QTermWidget
{
    Q_OBJECT
public:
    explicit QWoTermWidget(QWidget *parent=nullptr);

private
slots:
    void onTimeout();
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinish(int code);
    void onSendData(const QByteArray& buf);
    void onCopyToClipboard();
    void onPasteFromClipboard();

private:
    void contextMenuEvent(QContextMenuEvent *event);


private:
    QPointer<QProcess> m_process;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_copy;
    QPointer<QAction> m_paste;
};
