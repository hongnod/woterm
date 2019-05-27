#pragma once

#include <QVariant>

class QWoSetting
{
    Q_OBJECT
public:
    static void setValue(const QString& key, const QVariant& val);
    static QVariant value(const QString& key);

    Q_INVOKABLE void setValue(const QVariant& key, const QVariant& val);
    Q_INVOKABLE QVariant value(const QVariant& key) const;
};
