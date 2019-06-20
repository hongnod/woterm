#include "qwoutils.h"

#include <QObject>
#include<QLayout>
#include<QWidget>
#include <QSpacerItem>
#include <QDebug>
#include <QBoxLayout>

void QWoUtils::setLayoutVisible(QLayout *layout, bool visible)
{
    QBoxLayout *box = qobject_cast<QBoxLayout*>(layout);
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        QWidget *w = item->widget();
        if(w) {
            w->setVisible(visible);
        }
    }
}
