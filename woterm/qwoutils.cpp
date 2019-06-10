#include "qwoutils.h"

#include<QLayout>
#include<QWidget>

void QWoUtils::setLayoutVisible(QLayout *layout, bool visible)
{
    for (int i = 0; i < layout->count(); ++i) {
        QLayoutItem *item = layout->itemAt(i);
        QWidget *w = item->widget();
        if(w) {
            w->setVisible(visible);
        }
    }
}
