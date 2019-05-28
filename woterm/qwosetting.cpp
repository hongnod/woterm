#include "qwosetting.h"

#include<QSettings>
#include<QApplication>
#include<QDir>

QWoSetting::QWoSetting(QObject *parent)
    : QSettings(parent)
{

}

QWoSetting *QWoSetting::instance()
{
    static QString path = QDir::cleanPath(QApplication::applicationDirPath()+"/../opt/woterm.ini");
    static QSettings setting(path, QSettings::IniFormat);
    static QWoSetting gset(&setting);
    return &gset;
}

void QWoSetting::setValue(const QString &key, const QVariant &val)
{
    instance()->setValue(key, val);
    instance()->sync();
}

QVariant QWoSetting::value(const QString &key)
{
    return instance()->value(key);
}

void QWoSetting::set(const QString &key, const QVariant &val)
{
    instance()->setValue(key, val);
    instance()->sync();
}

QVariant QWoSetting::get(const QString &key) const
{
    return instance()->value(key);
}
