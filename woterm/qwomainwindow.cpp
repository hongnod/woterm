#include "qwomainwindow.h"
#include "qwosetting.h"
#include "qwoshower.h"
#include "qwowidget.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"
#include "qwosessionmanager.h"

#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTabBar>
#include <QToolBar>
#include <QPushButton>
#include <QDesktopServices>
#include <QDir>

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

    m_tool = new QToolBar(this);
    addToolBar(m_tool);

    m_manager = new QWoSessionManager("SessionManager", this);
    m_manager->setFloating(false);
    m_manager->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    m_manager->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget(Qt::LeftDockWidgetArea, m_manager);

    QWoWidget *central = new QWoWidget(this);
    setCentralWidget(central);

    m_tab = new QTabBar(this);
    m_tab->setMovable(true);
    m_tab->setTabsClosable(true);
    m_tab->setExpanding(false);
    m_tab->setUsesScrollButtons(true);
    m_shower = new QWoShower(m_tab, this);

    m_layout = new QVBoxLayout(central);
    central->setLayout(m_layout);
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_layout->addWidget(m_tab);
    m_layout->addWidget(m_shower);

    QAction *newTerm = m_tool->addAction("New");
    QObject::connect(newTerm, SIGNAL(triggered()), this, SLOT(onNewTerm()));

    QAction *openTerm = m_tool->addAction("Open");
    QObject::connect(openTerm, SIGNAL(triggered()), this, SLOT(onOpenTerm()));

    QAction *edit = m_tool->addAction("Edit");
    QObject::connect(edit, SIGNAL(triggered()), this, SLOT(onEditConfig()));
}

QWoMainWindow *QWoMainWindow::instance()
{
     static QWoMainWindow win;
     return &win;
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

void QWoMainWindow::onNewTerm()
{
//    QWoSshProcess *process = new QWoSshProcess();
//    QWoTermWidget *console = new QWoTermWidget(process, m_shower);
//    process.start();
//    QVariantMap mapVar;
//    mapVar["process"] = process;
}

void QWoMainWindow::onOpenTerm()
{
    m_shower->openConnection("target");
}

void QWoMainWindow::onEditConfig()
{
    QString cfg = QDir::cleanPath(QApplication::applicationDirPath() + "/../");
    QDesktopServices::openUrl(QUrl(cfg, QUrl::TolerantMode));
}
