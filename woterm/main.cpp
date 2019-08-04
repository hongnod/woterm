#include <QApplication>
#include <QtDebug>
#include <QIcon>
#include <QMenuBar>
#include <QProcess>
#include <QMessageBox>
#include <QList>

#include "qwoglobal.h"
#include "qwomainwindow.h"
#include "qwotermwidget.h"
#include "qwosshprocess.h"
#include "qwosetting.h"
#include "qwosshconf.h"

void test()
{

}

int main(int argc, char *argv[])
{
    static QApplication app(argc, argv);
    QIcon::setThemeName("oxygen");
    qputenv("TERM", "xterm-256color");
    QApplication::setStyle("Fusion");

    test();

    QWoMainWindow *mainWindow = QWoMainWindow::instance();
    mainWindow->show();

    QTimer::singleShot(1000, mainWindow, SLOT(onOpenSessionList()));

    return app.exec();
}
