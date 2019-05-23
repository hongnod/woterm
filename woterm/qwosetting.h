#pragma once

#include <QVariant>

class QWoSetting
{
public:
    static void setValue(const QString& key, const QVariant& v);
    static QVariant value(const QString& key);
};
