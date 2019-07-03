#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"

QWoSessionProperty::QWoSessionProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoSessionProperty)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}
