#include "qwoshower.h"
#include "qwosshprocess.h"
#include "qwotermwidget.h"

#include <QTabBar>
#include <QResizeEvent>

QWoShower::QWoShower(QTabBar *tab, QWidget *parent)
    : QWidget (parent)
    , m_tabs(tab)
{

}

bool QWoShower::openConnection(const QString &target)
{
    QWoSshProcess *process = new QWoSshProcess(target);
    QWoTermWidget *term = new QWoTermWidget(process, this);
    syncGeometry(term);
    process->start();
    term->show();
    m_terms.append(term);
    return true;
}

void QWoShower::resizeEvent(QResizeEvent *event)
{
    QSize newSize = event->size();
    QRect rt(0, 0, newSize.width(), newSize.height());
    for(int i = 0; i < m_terms.length(); i++) {
        QWoTermWidget *term = m_terms.at(i);
        term->setGeometry(rt);
    }
}

void QWoShower::syncGeometry(QWidget *widget)
{
    QRect rt = geometry();
    rt.moveTo(0, 0);
    widget->setGeometry(rt);
}
