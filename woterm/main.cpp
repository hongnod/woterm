#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QStyleFactory>
#include <QDebug>
#include <QQmlContext>
#include <QDir>

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
    //qputenv("QT_QUICK_CONTROLS_IMAGINE_PATH", ":/imagine-assets/");

    QQmlApplicationEngine engine;
    //engine.load(QUrl(QStringLiteral("qrc:/woterm.qml")));
    QQmlContext *qmlContext = engine.rootContext();
    QString skinPath = QDir::cleanPath(QApplication::applicationDirPath() +"/../opt/skin/");
    qmlContext->setContextProperty("skinPath", );
    engine.load(QUrl(QStringLiteral("qrc:/layout/musicplayer.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
