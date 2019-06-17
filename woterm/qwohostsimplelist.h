#ifndef QWOHOSTSIMPLELIST_H
#define QWOHOSTSIMPLELIST_H

#include "qwoglobal.h"

#include <QDialog>

namespace Ui {
class QWoHostList;
}

class QWoHostSimpleList : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostSimpleList(QWidget *parent = nullptr);
    ~QWoHostSimpleList();

private slots:

private:
    void init();
private:
    Ui::QWoHostList *ui;
};

#endif // QWOHOSTSIMPLELIST_H
