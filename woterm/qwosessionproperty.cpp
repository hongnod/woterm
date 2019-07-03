#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"

QWoSessionProperty::QWoSessionProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoSessionProperty)
{
    ui->setupUi(this);
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}
