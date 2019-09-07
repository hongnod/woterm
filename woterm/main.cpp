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
#include "qwotermstyle.h"

void test()
{
    freopen("c:\\slyar.txt", "w", stdout);
    int len = system("dir");
    qDebug() << "len" << len;
}

int main(int argc, char *argv[])
{
    static QApplication app(argc, argv);
    qputenv("TERM", "xterm-256color");
    QApplication::setStyle(new QWoTermStyle());

    test();

    QWoMainWindow *mainWindow = QWoMainWindow::instance();
    mainWindow->show();

    if(app.arguments().length() <= 1) {
        QTimer::singleShot(0, mainWindow, SLOT(onAppStart()));
    }

    return app.exec();
}
