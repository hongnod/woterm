#include "qwotermmask.h"
#include "ui_qwotermmask.h"

QWoTermMask::QWoTermMask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoTermMask)
{
    ui->setupUi(this);

    setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background, qRgba(128, 128, 128, 128));
    pal.setColor(QPalette::Window, qRgba(128, 128, 128, 128));
    setPalette(pal);
}

QWoTermMask::~QWoTermMask()
{
    delete ui;
}
