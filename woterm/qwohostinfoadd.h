#ifndef QWOHOSTINFOADD_H
#define QWOHOSTINFOADD_H

#include <QDialog>

namespace Ui {
class QWoHostInfoAdd;
}

class QWoHostInfoAdd : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostInfoAdd(QWidget *parent = nullptr);
    ~QWoHostInfoAdd();

private:
    Ui::QWoHostInfoAdd *ui;
};

#endif // QWOHOSTINFOADD_H
