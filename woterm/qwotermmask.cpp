#include "qwotermmask.h"
#include "ui_qwotermmask.h"

QWoTermMask::QWoTermMask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoTermMask)
{
    ui->setupUi(this);
}

QWoTermMask::~QWoTermMask()
{
    delete ui;
}
