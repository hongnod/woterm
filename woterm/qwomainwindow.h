#pragma once

#include "qwoglobal.h"

#include <QMainWindow>
#include <QPointer>

class QMenuBar;
class QVBoxLayout;
class QTabBar;
class QToolBar;
class QWoShower;
class QWoSessionManager;

namespace Ui {
class QWoMainWindow;
}


class QWoMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QWoMainWindow(QWidget *parent=nullptr);
    virtual ~QWoMainWindow();
    static QWoMainWindow *instance();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void onNewTerm();
    void onOpenTerm();
    void onEditConfig();
    void onSessionDoubleClicked(const HostInfo& hi);
    void onSessionBatchClicked(const QVariantList& his);
    void onProcessStartCheck();

private slots:
    void onActionNewTriggered();
    void onActionOpenTriggered();
    void onActionDisconnectTriggered();
    void onActionReconnectTriggered();
    void onActionReconnectAllTriggered();
    void onActionImportTriggered();
    void onActionExportTriggered();
    void onActionTransferTriggered();
    void onActionLogTriggered();
    void onActionExitTriggered();

private:
    Ui::QWoMainWindow *ui;
    QPointer<QTabBar> m_tab;
    QPointer<QWoSessionManager> m_manager;
    QPointer<QWoShower> m_shower;
};
