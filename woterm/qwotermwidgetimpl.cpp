#include "qwotermwidgetimpl..h"

#include <QCloseEvent>
#include <QSplitter>


QWoTermWidgetImpl::QWoTermWidgetImpl(QString target, QWidget *parent)
    : QWoWidget (parent)
    , m_target(target)
{
    m_splitter = new QSplitter(this);
}

void QWoTermWidgetImpl::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
