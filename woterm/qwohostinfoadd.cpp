#include "qwohostinfoadd.h"
#include "ui_qwohostinfoadd.h"

QWoHostInfoAdd::QWoHostInfoAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoAdd)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

QWoHostInfoAdd::~QWoHostInfoAdd()
{
    delete ui;
}
