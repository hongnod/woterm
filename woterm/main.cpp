#include <QApplication>
#include <QtDebug>
#include <QIcon>
#include <QMenuBar>
#include <QProcess>
#include <QMessageBox>

#include "qwomainwindow.h"
#include "qwotermwidget.h"
#include "qwosshprocess.h"
#include "qwosetting.h"

void test()
{

}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QIcon::setThemeName("oxygen");
    qputenv("TERM", "xterm-256color");
    QApplication::setStyle("Fusion");

    test();

    QWoMainWindow *mainWindow = QWoMainWindow::instance();
    mainWindow->show();

    return app.exec();
}
