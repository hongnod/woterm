#include "qwotermstyle.h"

#include <QStyleFactory>

QWoTermStyle::QWoTermStyle()
    : QProxyStyle(QStyleFactory::create("Fusion"))
{

}
