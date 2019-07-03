#ifndef QWOSESSIONPROPERTY_H
#define QWOSESSIONPROPERTY_H

#include <QDialog>

namespace Ui {
class QWoSessionProperty;
}

class QWoSessionProperty : public QDialog
{
    Q_OBJECT

public:
    explicit QWoSessionProperty(QWidget *parent = nullptr);
    ~QWoSessionProperty();

private:
    Ui::QWoSessionProperty *ui;
};

#endif // QWOSESSIONPROPERTY_H
