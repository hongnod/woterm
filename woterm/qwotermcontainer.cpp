#include "qwotermcontainer.h"

#include <QCloseEvent>


QWoTermContainer::QWoTermContainer(QWidget *parent)
    : QWoWidget (parent)
{

}

void QWoTermContainer::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
