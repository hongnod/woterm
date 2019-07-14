#pragma once

#include <QObject>
#include <QUuid>

typedef struct{
    QString name;
    QString host;
    int port;
    QString user;
    QString password;
    QString identityFile;
    QString proxyJump;
    QString memo;
}HostInfo;

Q_DECLARE_METATYPE(HostInfo)

#ifdef Q_OS_MACOS
#define DEFAULT_FONT_FAMILY "Menlo"
#elif defined(Q_OS_WIN)
//#define DEFAULT_FONT_FAMILY "Lucida Console"
#define DEFAULT_FONT_FAMILY "Courier New"
#else
#define DEFAULT_FONT_FAMILY "Monospace"
#endif
#define DEFAULT_FONT_SIZE (10)
#define DEFAULT_COLOR_SCHEMA "Linux"
#define DEFAULT_KEYBOARD_BINDING "linux"
#define DEFAULT_HISTORY_LINE_LENGTH  (1000)
