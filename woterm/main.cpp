#include "qwoitem.h"
#include "qwosetting.h"
#include "qwomainwindow.h"

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
    qmlRegisterType<QWoSetting>("WoSetting", 1, 0, "WoSetting");

    QStringList styles = QQuickStyle::availableStyles();
    qDebug() << "styles:" << styles;
    QQuickStyle::setStyle("Imagine");
    QQuickStyle::setFallbackStyle("Default");

    QQuickWidget *quick = new QQuickWidget(mainWindow);
    quick->setResizeMode(QQuickWidget::SizeRootObjectToView );
    quick->setSource(QUrl("qrc:/woterm.qml"));
    mainWindow->setCentralWidget(quick);
    mainWindow->show();
    return app.exec();
}

int main2(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("WoTerm");
    QGuiApplication::setOrganizationName("WoTerm");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);

    QStringList styles = QQuickStyle::availableStyles();
    qDebug() << "styles:" << styles;
    QQuickStyle::setStyle("Imagine");
    QQuickStyle::setFallbackStyle("Default");

    qmlRegisterType<QWoItem>("WoItem", 1, 0, "WoItem");
    qmlRegisterType<QWoSetting>("WoSetting", 1, 0, "WoSetting");

    QQmlApplicationEngine engine;

    QQmlContext *qmlContext = engine.rootContext();
    qmlContext->setContextProperty("conf", QWoSetting::instance());
#ifdef USE_CUSTOM_SKIN
    QString opt = QDir::cleanPath(QApplication::applicationDirPath() +"/../opt/");
    QString conf = QDir::cleanPath(opt + "/skins/skin.conf");
    QString skinPath = QDir::cleanPath(opt + "/skins/imagine");
    qputenv("QT_QUICK_CONTROLS_CONF", conf.toUtf8());
    //qputenv("QT_QUICK_CONTROLS_HOVER_ENABLED", "0");
    qmlContext->setContextProperty("skinPath", skinPath);
#endif
    engine.load(QUrl(QStringLiteral("qrc:/woterm.qml")));
    //engine.load(QUrl(QStringLiteral("qrc:/layout/musicplayer.qml")));
    QList<QObject*> objList = engine.rootObjects();
    if (objList.isEmpty())
        return -1;
    for(int i = 0; i < objList.length(); i++) {
        QObject *obj = objList.at(i);
        QWidget *ow = qobject_cast<QWidget*>(obj);
        if(ow) {
            qDebug() << ow;
        }
    }
    return app.exec();
}
