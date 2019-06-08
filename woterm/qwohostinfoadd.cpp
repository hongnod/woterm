#include "qwohostinfoadd.h"
#include "ui_qwohostinfoadd.h"

QWoHostInfoAdd::QWoHostInfoAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoAdd)
{
    ui->setupUi(this);
}

QWoHostInfoAdd::~QWoHostInfoAdd()
{
    delete ui;
}
