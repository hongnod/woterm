#ifndef QWOHOSTINFOMODIFY_H
#define QWOHOSTINFOMODIFY_H

#include <QDialog>

namespace Ui {
class QWoHostInfoModify;
}

class QWoHostInfoModify : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostInfoModify(QWidget *parent = nullptr);
    ~QWoHostInfoModify();

private:
    Ui::QWoHostInfoModify *ui;
};

#endif // QWOHOSTINFOMODIFY_H
