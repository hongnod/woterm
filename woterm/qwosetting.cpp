#include "qwosetting.h"

#include<QSettings>
#include<QApplication>
#include<QDir>

QSettings *qSettings() {
    static QString path = QDir::cleanPath(QApplication::applicationDirPath()+"/../opt/woterm.ini");
    static QSettings setting(path, QSettings::IniFormat);
    return &setting;
}

QWoSetting::QWoSetting(){}

QWoSetting *QWoSetting::instance()
{
    static QWoSetting gset;
    return &gset;
}

void QWoSetting::setValue(const QString &key, const QVariant &val)
{
    qSettings()->setValue(key, val);
    qSettings()->sync();
}

QVariant QWoSetting::value(const QString &key)
{
    return qSettings()->value(key);
}

void QWoSetting::set(const QString &key, const QVariant &val)
{
    qSettings()->setValue(key, val);
    qSettings()->sync();
}

QVariant QWoSetting::get(const QString &key) const
{
    return qSettings()->value(key);
}
