#include "qwotermstyle.h"

#include <QtWidgets>
#include <QStyleFactory>

QWoTermStyle::QWoTermStyle()
    : QProxyStyle(QStyleFactory::create("Fusion"))
{

}

void QWoTermStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    switch (element) {
    case CE_TabBarTab:
        {
            QStyleOptionTab myOption;
            const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option);
            if (tabOption) {

            }
            QProxyStyle::drawControl(element, tabOption, painter, widget);
        }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}
