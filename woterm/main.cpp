#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QStyleFactory>
#include <QDebug>
#include <QQmlContext>
#include <QDir>
#include <QVariant>

#define USE_CUSTOM_SKIN

int main(int argc, char *argv[])
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

    QQmlApplicationEngine engine;

    QQmlContext *qmlContext = engine.rootContext();
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
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
