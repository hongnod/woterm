#include "qwohostinfomodify.h"
#include "ui_qwohostinfomodify.h"

QWoHostInfoModify::QWoHostInfoModify(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoModify)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

QWoHostInfoModify::~QWoHostInfoModify()
{
    delete ui;
}
