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

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    static QApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif
    QApplication::setStyle(new QWoTermStyle());
    QApplication::setWindowIcon(QIcon(":/qwoterm/resource/skin/woterm4.png"));

    QWoMainWindow *mainWindow = QWoMainWindow::instance();
    mainWindow->show();

    if(app.arguments().length() <= 1) {
        QTimer::singleShot(0, mainWindow, SLOT(onAppStart()));
    }

    return app.exec();
}
