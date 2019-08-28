#pragma once

#include "qwoglobal.h"

#include <QMainWindow>
#include <QPointer>

class QMenuBar;
class QVBoxLayout;
class QTabBar;
class QToolBar;
class QWoShower;
class QWoSessionList;

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
    void onSessionReadyToConnect(const QString& target);
    void onSessionBatchToConnect(const QStringList& targets);
    void onProcessStartCheck();
    void onAppStart();
    void onShouldAppExit();

private slots:
    void onActionNewTriggered();
    void onActionOpenTriggered();
    void onActionDisconnectTriggered();
    void onActionReconnectTriggered();
    void onActionReconnectAllTriggered();
    void onActionImportTriggered();
    void onActionExportTriggered();
    void onActionSaveTriggered();
    void onActionTransferTriggered();
    void onActionLogTriggered();
    void onActionExitTriggered();
    void onActionDefaultTriggered();
    void onActionFindTriggered();

private:
    Ui::QWoMainWindow *ui;
    QPointer<QTabBar> m_tab;
    QPointer<QWoSessionList> m_sessions;
    QPointer<QWoShower> m_shower;
};
