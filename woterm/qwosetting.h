#pragma once

#include <QObject>
#include <QVariant>

class QWoSetting : public QObject
{
    Q_OBJECT
public:
    explicit QWoSetting();
    static QWoSetting *instance();
    static void setValue(const QString& key, const QVariant& val);
    static QVariant value(const QString& key);

    Q_INVOKABLE void set(const QString& key, const QVariant& val);
    Q_INVOKABLE QVariant get(const QString& key) const;
};
