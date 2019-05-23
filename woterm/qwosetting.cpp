#include "qwosetting.h"


QWoSetting::QWoSetting()
    : QSettings (QSettings::IniFormat, QSettings::SystemScope, "woterm", "woterm")
{

}

QWoSetting *QWoSetting::instance()
{
   static QWoSetting setting;
   return &setting;
}
