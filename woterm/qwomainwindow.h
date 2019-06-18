#pragma once

#include <QMainWindow>
#include <QPointer>

class QMenuBar;
class QVBoxLayout;
class QTabBar;
class QToolBar;
class QWoShower;
class QWoSessionManager;

class QWoMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit QWoMainWindow(QWidget *parent=nullptr);
    static QWoMainWindow *instance();
protected:
    void closeEvent(QCloseEvent *event);
private slots:
    void onNewTerm();
    void onOpenTerm();
    void onEditConfig();
    void onSessionDoubleClicked(const QString& name, int idxInCfg);
    void onSessionBatchClicked(const QStringList& sessions);
    void onProcessStartCheck();

private:
    QPointer<QMenuBar> m_memuBar;
    QPointer<QToolBar> m_tool;
    QPointer<QVBoxLayout> m_layout;
    QPointer<QTabBar> m_tab;
    QPointer<QWoSessionManager> m_manager;
    QPointer<QWoShower> m_shower;

};
