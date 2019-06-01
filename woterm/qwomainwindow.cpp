#include "qwomainwindow.h"
#include "qwosetting.h"
#include "qwoshower.h"

#include <QMessageBox>
#include <QCloseEvent>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTabBar>
#include <QToolBar>

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
    m_tool->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    m_tab = new QTabBar(this);
    m_tab->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

    m_shower = new QWoShower(this);
    m_layout = new QVBoxLayout(this);
    setLayout(m_layout);

    m_layout->addWidget(m_tool);
    m_layout->addWidget(m_tab);
    m_layout->addWidget(m_shower);

    m_tool->addAction("AB");
    m_tool->addAction("AB2");
    m_tool->addAction("AB3");
    m_tab->addTab("A");
    m_tab->addTab("B");
    m_tab->addTab("C");
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
