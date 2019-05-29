#include "qwoitem.h"
#include "qwosetting.h"
#include "qwomainwindow.h"
#include "qwotermitem.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QStyleFactory>
#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QVariant>
#include <QPalette>
#include <QWidget>
#include <QQuickWidget>
#include <QMenuBar>
#include <QMenu>


//#define USE_CUSTOM_SKIN

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("WoTerm");
    QGuiApplication::setOrganizationName("WoTerm");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
    QIcon::setThemeName("oxygen");
    qputenv("TERM", "xterm-256color");
    QWoMainWindow *mainWindow = new QWoMainWindow();
    QMenuBar *menuBar = new QMenuBar(mainWindow);
    QMenu *actionsMenu = new QMenu("Actions", menuBar);
    menuBar->addMenu(actionsMenu);
    actionsMenu->addAction("Find...");
    actionsMenu->addAction("About Qt");
    mainWindow->setMenuBar(menuBar);
    mainWindow->setMenuBar(menuBar);

    qmlRegisterType<QWoItem>("WoItem", 1, 0, "WoItem");
    qmlRegisterType<QWoTermItem>("WoTermItem", 1, 0, "WoTermItem");

    QStringList styles = QQuickStyle::availableStyles();
    qDebug() << "styles:" << styles;
    QQuickStyle::setStyle("Imagine");
    QQuickStyle::setFallbackStyle("Default");

    QQuickWidget *quick = QWoTermItem::container();
    quick->setParent(mainWindow);
    quick->setResizeMode(QQuickWidget::SizeRootObjectToView );
    quick->setSource(QUrl("qrc:/woterm.qml"));
    QQmlContext *qmlContext = quick->rootContext();
    qmlContext->setContextProperty("conf", QWoSetting::instance());
    qmlContext->setContextProperty("container", quick);

    mainWindow->setCentralWidget(quick);
    mainWindow->show();
#ifdef USE_CUSTOM_SKIN
    QString opt = QDir::cleanPath(QApplication::applicationDirPath() +"/../opt/");
    QString conf = QDir::cleanPath(opt + "/skins/skin.conf");
    QString skinPath = QDir::cleanPath(opt + "/skins/imagine");
    qputenv("QT_QUICK_CONTROLS_CONF", conf.toUtf8());
    //qputenv("QT_QUICK_CONTROLS_HOVER_ENABLED", "0");
    qmlContext->setContextProperty("skinPath", skinPath);
#endif
    return app.exec();
}
