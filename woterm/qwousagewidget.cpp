#include "qwousagewidget.h"
#include "ui_qwousagewidget.h"

QWoUsageWidget::QWoUsageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoUsageWidget)
{
    ui->setupUi(this);
}

QWoUsageWidget::~QWoUsageWidget()
{
    delete ui;
}
