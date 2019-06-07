#include "qwohosteditdialog.h"
#include "ui_qwohosteditdialog.h"

QWoHostEditDialog::QWoHostEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostEditDialog)
{
    ui->setupUi(this);
}

QWoHostEditDialog::~QWoHostEditDialog()
{
    delete ui;
}
