#include "qwohostinfomodify.h"
#include "ui_qwohostinfomodify.h"

QWoHostInfoModify::QWoHostInfoModify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoModify)
{
    ui->setupUi(this);
}

QWoHostInfoModify::~QWoHostInfoModify()
{
    delete ui;
}
