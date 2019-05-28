#include "qwotermitem.h"
#include "qwotermwidget.h"

#include <QQuickWidget>


QWoTermItem::QWoTermItem(QQuickItem *parent)
    : QWoItem(parent)
{
    QWindow *widget = window();
}

QWoTermItem::~QWoTermItem()
{

}
