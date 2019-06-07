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
#include "qwosshconf.h"

void test()
{
    QWoSshConf conf;
    if(!conf.load("d:/config")) {
        return;
    }
    conf.save("d:/config_save");
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
