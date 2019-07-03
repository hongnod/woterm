#include "qwomainwindow.h"
#include "qwosetting.h"
#include "qwoshower.h"
#include "qwowidget.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"
#include "qwosessionmanager.h"
#include "ui_qwomainwindow.h"
#include "qwosessionproperty.h"

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

Q_GLOBAL_STATIC(QWoMainWindow, mainWindow)

QWoMainWindow::QWoMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::QWoMainWindow)
{
    ui->setupUi(this);
    QByteArray geom = QWoSetting::value("woterm/geometry").toByteArray();
    restoreGeometry(geom);

    QObject::connect(ui->actionDisconect, SIGNAL(triggered()), this, SLOT(onActionDisconnectTriggered()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(onActionExitTriggered()));
    QObject::connect(ui->actionExport, SIGNAL(triggered()), this, SLOT(onActionExportTriggered()));
    QObject::connect(ui->actionImport, SIGNAL(triggered()), this, SLOT(onActionImportTriggered()));
    QObject::connect(ui->actionLog, SIGNAL(triggered()), this, SLOT(onActionLogTriggered()));
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(onActionNewTriggered()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onActionOpenTriggered()));
    QObject::connect(ui->actionReconnect, SIGNAL(triggered()), this, SLOT(onActionReconnectTriggered()));
    QObject::connect(ui->actionReconnectAll, SIGNAL(triggered()), this, SLOT(onActionReconnectAllTriggered()));
    QObject::connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onActionSaveTriggered()));
    QObject::connect(ui->actionTransfer, SIGNAL(triggered()), this, SLOT(onActionTransferTriggered()));

    //QMenu *actionsMenu = new QMenu("Actions", ui->menuBar);
    //ui->menuBar->addMenu(actionsMenu);
    //actionsMenu->addAction("Find...", this, SLOT(toggleShowSearchBar()), QKeySequence(Qt::CTRL +  Qt::Key_F));
    //actionsMenu->addAction("About Qt", this, SLOT(aboutQt()));

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

    QVBoxLayout *layout = new QVBoxLayout(central);
    central->setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    layout->addWidget(m_tab);
    layout->addWidget(m_shower);

    QToolBar *tool = ui->mainToolBar;
    QAction *newTerm = tool->addAction("New");
    QObject::connect(newTerm, SIGNAL(triggered()), this, SLOT(onNewTerm()));

    QAction *openTerm = tool->addAction("Open");
    QObject::connect(openTerm, SIGNAL(triggered()), this, SLOT(onOpenTerm()));

    QAction *edit = tool->addAction("Edit");
    QObject::connect(edit, SIGNAL(triggered()), this, SLOT(onEditConfig()));

    QObject::connect(m_manager, SIGNAL(sessionDoubleClicked(const HostInfo&)), this, SLOT(onSessionDoubleClicked(const HostInfo&)));
    QObject::connect(m_manager, SIGNAL(sessionBatchClicked(const QVariantList&)), this, SLOT(onSessionBatchClicked(const QVariantList&)));

    QTimer::singleShot(1000, this, SLOT(onProcessStartCheck()));
}

QWoMainWindow::~QWoMainWindow()
{
    delete ui;
}

QWoMainWindow *QWoMainWindow::instance()
{
     return mainWindow;
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

void QWoMainWindow::onActionNewTriggered()
{
    QWoSessionProperty dlg;
    dlg.exec();
}

void QWoMainWindow::onActionOpenTriggered()
{

}

void QWoMainWindow::onActionDisconnectTriggered()
{

}

void QWoMainWindow::onActionReconnectTriggered()
{

}

void QWoMainWindow::onActionReconnectAllTriggered()
{

}

void QWoMainWindow::onActionImportTriggered()
{

}

void QWoMainWindow::onActionExportTriggered()
{

}

void QWoMainWindow::onActionSaveTriggered()
{

}

void QWoMainWindow::onActionTransferTriggered()
{

}

void QWoMainWindow::onActionLogTriggered()
{

}

void QWoMainWindow::onActionExitTriggered()
{

}
