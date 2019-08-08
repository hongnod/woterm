#ifndef QWOTERMMASK_H
#define QWOTERMMASK_H

#include <QWidget>

namespace Ui {
class QWoTermMask;
}

class QWoTermMask : public QWidget
{
    Q_OBJECT

public:
    explicit QWoTermMask(QWidget *parent = nullptr);
    ~QWoTermMask();

private:
    Ui::QWoTermMask *ui;
};

#endif // QWOTERMMASK_H
