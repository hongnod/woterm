#include "qwosetting.h"

#include<QSettings>
#include<QApplication>
#include<QDir>

QSettings *qSettings()
{
    static QString path = QDir::cleanPath(QApplication::applicationDirPath()+"/../opt/woterm.ini");
    static QSettings setting(path, QSettings::IniFormat);
    return &setting;
}

void QWoSetting::setValue(const QString &key, const QVariant &v)
{
    qSettings()->setValue(key, v);
    qSettings()->sync();
}

QVariant QWoSetting::value(const QString &key)
{
    return qSettings()->value(key);
}

QString QWoSetting::zmodemSZPath()
{
    QString path;
    path = QWoSetting::value("zmodem/sz").toString();
    if(!QFile::exists(path)) {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/sz");
#ifdef Q_OS_WIN
        path.append(".exe");
#endif
        if(!QFile::exists(path)){
            return "";
        }
    }
    return path;
}

QString QWoSetting::zmodemRZPath()
{
    QString path;
    path = QWoSetting::value("zmodem/rz").toString();
    if(!QFile::exists(path)) {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/rz");
#ifdef Q_OS_WIN
        path.append(".exe");
#endif
        if(!QFile::exists(path)){
            return "";
        }
    }
    return path;
}

QString QWoSetting::sshProgramPath()
{
    QString path;
    path = QWoSetting::value("ssh/program").toString();
    if(!QFile::exists(path)) {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/ssh");
#ifdef Q_OS_WIN
        path.append(".exe");
#endif
        if(!QFile::exists(path)){
            return "";
        }
    }
    return path;
}

QString QWoSetting::sshServerListPath()
{
    QString path;
    path = QWoSetting::value("ssh/serverList").toString();
    if(!QFile::exists(path)) {
        path = QDir::cleanPath(QApplication::applicationDirPath() + "/../config");
        if(!QFile::exists(path)){
            return "";
        }
    }
    return path;
}
