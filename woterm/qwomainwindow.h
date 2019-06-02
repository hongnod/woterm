#pragma once

#include <QMainWindow>
#include <QPointer>

class QMenuBar;
class QVBoxLayout;
class QTabBar;
class QToolBar;
class QWoShower;

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

private:
    QPointer<QMenuBar> m_memuBar;
    QPointer<QToolBar> m_tool;
    QPointer<QVBoxLayout> m_layout;
    QPointer<QTabBar> m_tab;
    QPointer<QWoShower> m_shower;

};
