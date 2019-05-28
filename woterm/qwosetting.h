#pragma once

#include <QSettings>

class QWoSetting : public QSettings
{
    Q_OBJECT
public:
    explicit QWoSetting(QObject *parent=nullptr);

    static QWoSetting *instance();
    static void setValue(const QString& key, const QVariant& val);
    static QVariant value(const QString& key);

    Q_INVOKABLE void set(const QString& key, const QVariant& val);
    Q_INVOKABLE QVariant get(const QString& key) const;
};
