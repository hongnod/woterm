#include "qwomainwindow.h"
#include "qwosetting.h"
#include "qwoshower.h"
#include "qwowidget.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTabBar>
#include <QToolBar>
#include <QPushButton>

QWoMainWindow::QWoMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QByteArray geom = QWoSetting::value("woterm/geometry").toByteArray();
    restoreGeometry(geom);

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *actionsMenu = new QMenu("Actions", menuBar);
    menuBar->addMenu(actionsMenu);
    actionsMenu->addAction("Find...", this, SLOT(toggleShowSearchBar()), QKeySequence(Qt::CTRL +  Qt::Key_F));
    actionsMenu->addAction("About Qt", this, SLOT(aboutQt()));
    setMenuBar(menuBar);

    QWoWidget *central = new QWoWidget(this);
    setCentralWidget(central);

    m_tool = new QToolBar(this);
    m_tab = new QTabBar(this);
    m_tab->setMovable(true);
    m_tab->setTabsClosable(true);
    m_tab->setExpanding(false);
    m_tab->setUsesScrollButtons(true);
    m_shower = new QWoShower(this);

    m_layout = new QVBoxLayout(central);
    central->setLayout(m_layout);

    m_layout->addWidget(m_tool);
    m_layout->addWidget(m_tab);
    m_layout->addWidget(m_shower);

    m_tool->addAction("AB");
    m_tool->addAction("AB2");
    m_tool->addAction("AB3");
    m_tab->addTab("Aadddfdfsdf");
    m_tab->addTab("Baadfsdfdsfdsf");
    m_tab->addTab("Cccccccc");
    m_tab->addTab("Cddddddd");
    m_tab->addTab("Cffffff");
    m_tab->addTab("Ceeeeee");
}

void QWoMainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton btn = QMessageBox::warning(this, "exit", "Exit Or Not?", QMessageBox::Ok|QMessageBox::No);
    if(btn == QMessageBox::No) {
        event->setAccepted(false);
        return ;
    }
    QByteArray geom = saveGeometry();
    QWoSetting::setValue("woterm/geometry", geom);
    QMainWindow::closeEvent(event);
}
