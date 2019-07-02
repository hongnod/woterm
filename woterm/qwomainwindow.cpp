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
#include <QDockWidget>

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

    m_tool = new QToolBar("ToolBar", this);
    addToolBar(m_tool);
    m_tool->hide();

    QDockWidget* dock = new QDockWidget("SessionManager", this);
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setFloating(false);
    dock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_manager = new QWoSessionManager(dock);
    dock->setWidget(m_manager);

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

    QObject::connect(m_manager, SIGNAL(sessionDoubleClicked(const HostInfo&)), this, SLOT(onSessionDoubleClicked(const HostInfo&)));
    QObject::connect(m_manager, SIGNAL(sessionBatchClicked(const QVariantList&)), this, SLOT(onSessionBatchClicked(const QVariantList&)));

    QTimer::singleShot(1000, this, SLOT(onProcessStartCheck()));
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

void QWoMainWindow::onSessionDoubleClicked(const HostInfo &hi)
{
    m_shower->openConnection(hi.name);
}

void QWoMainWindow::onSessionBatchClicked(const QVariantList &his)
{
    for(int i = 0; i < his.length(); i++) {
        const HostInfo& hi = his.at(i).value<HostInfo>();
        m_shower->openConnection(hi.name);
    }
}

void QWoMainWindow::onProcessStartCheck()
{
    QStringList args = QApplication::arguments();
    args.takeFirst();
    if(args.isEmpty()) {
        return;
    }
    for(int i = 0; i < args.length(); i++) {
        m_shower->openConnection(args.at(i));
    }
}
