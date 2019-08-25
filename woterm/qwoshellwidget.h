#pragma once

#include "qwolinenoise.h"

#include <qtermwidget.h>

#include <QPointer>
#include <QDir>

class QWoProcess;
class QMenu;
class QWoShellWidgetImpl;


class QWoShellWidget : public QTermWidget, QWoLineNoise
{
    Q_OBJECT
public:
    explicit QWoShellWidget(QWidget *parent=nullptr);
    virtual ~QWoShellWidget();

    void closeAndDelete();

signals:
    void aboutToClose(QCloseEvent* event);

private
slots:
    void onTimeout();
    void onSendData(const QByteArray& buf);
    void onCopyToClipboard();
    void onPasteFromClipboard();
    void onVerticalSplitView();
    void onHorizontalSplitView();
    void onCloseThisSession();
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    void initDefault();
    void resetProperty(QVariantMap data);
    void splitWidget(bool vertical);
    void loadCommandList();
    void showWellcome();
    void refreshPrompt();
protected:
    void handleCommand(const QByteArray& line);
    QList<QByteArray> handleComplete(const QByteArray& line);
    QByteArray handleShowHints(QByteArray& line, int *pclr, int *pbold);
private:
    friend class QWoShellWidgetImpl;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_copy;
    QPointer<QAction> m_paste;
    QPointer<QWoLineNoise> m_linenoise;
    bool m_bexit;
    bool m_bScrollToEnd;

    QMap<QByteArray, QString> m_cmds;

    QDir m_dirCurrent;
};
