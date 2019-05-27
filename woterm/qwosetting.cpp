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

void QWoSetting::setValue(const QString &key, const QVariant &val)
{
    qSettings()->setValue(key, val);
    qSettings()->sync();
}

QVariant QWoSetting::value(const QString &key)
{
    return qSettings()->value(key);
}

void QWoSetting::setValue(const QVariant &key, const QVariant &val)
{
    qSettings()->setValue(key.toString(), val);
    qSettings()->sync();
}

QVariant QWoSetting::value(const QVariant &key) const
{
    return qSettings()->value(key.toString());
}
