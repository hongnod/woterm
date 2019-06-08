#include "qwohostinfolist.h"
#include "ui_qwohostinfolist.h"

QWoHostInfoList::QWoHostInfoList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoList)
{
    ui->setupUi(this);
}

QWoHostInfoList::~QWoHostInfoList()
{
    delete ui;
}
