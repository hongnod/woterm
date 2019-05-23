#pragma once

#include <QSettings>
#include <QPointer>

class QWoSetting : public QSettings
{
    Q_OBJECT
public:
    explicit QWoSetting();
    static QWoSetting *instance();
private:
    QPointer<QSettings> m_setting;
};
