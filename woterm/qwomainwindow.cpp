#include "qwomainwindow.h"
#include "qwosetting.h"
#include "qwoshower.h"
#include "qwowidget.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"
#include "qwosessionlist.h"
#include "ui_qwomainwindow.h"
#include "qwosessionproperty.h"
#include "qwosessionmanage.h"

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
    setContentsMargins(3,3,3,3);
    setWindowTitle("WoTerm");

    initMenuBar();
    initToolBar();
    initStatusBar();
    //QMenu *actionsMenu = new QMenu("Actions", ui->menuBar);
    //ui->menuBar->addMenu(actionsMenu);
    //actionsMenu->addAction("Find...", this, SLOT(toggleShowSearchBar()), QKeySequence(Qt::CTRL +  Qt::Key_F));
    //actionsMenu->addAction("About Qt", this, SLOT(aboutQt()));

    m_dock = new QDockWidget("SessionManager", this);
    addDockWidget(Qt::LeftDockWidgetArea, m_dock);
    m_dock->setFloating(false);
    m_dock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    m_dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);

    m_sessions = new QWoSessionList(m_dock);
    m_dock->setWidget(m_sessions);

    QWoWidget *central = new QWoWidget(this);
    setCentralWidget(central);

    m_tab = new QTabBar(this);
    m_tab->setMovable(true);
    m_tab->setTabsClosable(true);
    m_tab->setExpanding(false);
    m_tab->setUsesScrollButtons(true);
    m_shower = new QWoShower(m_tab, this);

    QObject::connect(m_shower, SIGNAL(tabEmpty()), this, SLOT(onShouldAppExit()));

    QVBoxLayout *layout = new QVBoxLayout(central);
    central->setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    layout->addWidget(m_tab);
    layout->addWidget(m_shower);

    QObject::connect(m_sessions, SIGNAL(readyToConnect(const QString&)), this, SLOT(onSessionReadyToConnect(const QString&)));
    QObject::connect(m_sessions, SIGNAL(batchReadyToConnect(const QStringList&)), this, SLOT(onSessionBatchToConnect(const QStringList&)));

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
    QWoSessionProperty dlg(QWoSessionProperty::NewSession, -1, this);
    QObject::connect(&dlg, SIGNAL(connect(const QString&)), this, SLOT(onSessionReadyToConnect(const QString&)));
    dlg.exec();
}

void QWoMainWindow::onOpenTerm()
{
    QWoSessionManage dlg(this);
    QObject::connect(&dlg, SIGNAL(connect(const QString&)), this, SLOT(onSessionReadyToConnect(const QString&)));
    dlg.exec();
}

void QWoMainWindow::onLayout()
{
    m_dock->setVisible(!m_dock->isVisible());
}

void QWoMainWindow::onEditConfig()
{
    QString cfg = QDir::cleanPath(QApplication::applicationDirPath() + "/../");
    QDesktopServices::openUrl(QUrl(cfg, QUrl::TolerantMode));
}

void QWoMainWindow::onSessionReadyToConnect(const QString &target)
{
    m_shower->openConnection(target);
}

void QWoMainWindow::onSessionBatchToConnect(const QStringList &targets)
{
    for(int i = 0; i < targets.length(); i++) {
        m_shower->openConnection(targets.at(i));
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

void QWoMainWindow::onAppStart()
{
    m_shower->openLocalShell();
}

void QWoMainWindow::onShouldAppExit()
{
    if(m_shower->tabCount()) {
        return;
    }
    QApplication::exit();
}

void QWoMainWindow::onActionNewTriggered()
{
    QWoSessionProperty dlg(QWoSessionProperty::NewSession, -1, this);
    QObject::connect(&dlg, SIGNAL(connect(const QString&)), this, SLOT(onSessionReadyToConnect(const QString&)));
    dlg.exec();
}

void QWoMainWindow::onActionOpenTriggered()
{
    QWoSessionManage dlg(this);
    QObject::connect(&dlg, SIGNAL(connect(const QString&)), this, SLOT(onSessionReadyToConnect(const QString&)));
    dlg.exec();
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

void QWoMainWindow::onActionConfigDefaultTriggered()
{
    QWoSessionProperty dlg(QWoSessionProperty::ModifyWithNoConnect, 0, this);
    dlg.exec();
}

void QWoMainWindow::onActionFindTriggered()
{
    m_shower->openFindDialog();
}

void QWoMainWindow::initMenuBar()
{
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
    QObject::connect(ui->actionDefault, SIGNAL(triggered()), this, SLOT(onActionConfigDefaultTriggered()));
    QObject::connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(onActionFindTriggered()));
    setMenuBar(nullptr);
}

void QWoMainWindow::initToolBar()
{
    QToolBar *tool = ui->mainToolBar;
    QAction *newTerm = tool->addAction(QIcon(":/qwoterm/resource/skin/add.png"), tr("New"));
    QObject::connect(newTerm, SIGNAL(triggered()), this, SLOT(onNewTerm()));

    QAction *openTerm = tool->addAction(QIcon(":/qwoterm/resource/skin/manage.png"), tr("Manage"));
    QObject::connect(openTerm, SIGNAL(triggered()), this, SLOT(onOpenTerm()));

    QAction *lay = tool->addAction(QIcon(":/qwoterm/resource/skin/list.png"), tr("List"));
    QObject::connect(lay, SIGNAL(triggered()), this, SLOT(onLayout()));

//    QAction *import = tool->addAction(QIcon(":/qwoterm/resource/skin/import.png"), tr("Import"));
//    QObject::connect(import, SIGNAL(triggered()), this, SLOT(onActionImportTriggered()));

//    QAction *myexport = tool->addAction(QIcon(":/qwoterm/resource/skin/export.png"), tr("Export"));
//    QObject::connect(myexport, SIGNAL(triggered()), this, SLOT(onActionExportTriggered()));

    QAction *cfgdef = tool->addAction(QIcon(":/qwoterm/resource/skin/cfgdef.png"), tr("Setting"));
    QObject::connect(cfgdef, SIGNAL(triggered()), this, SLOT(onActionConfigDefaultTriggered()));
}

void QWoMainWindow::initStatusBar()
{
    //QStatusBar *bar = ui->statusBar;
    setStatusBar(nullptr);
}
