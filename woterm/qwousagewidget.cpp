#include "qwousagewidget.h"
#include "ui_qwousagewidget.h"

QWoUsageWidget::QWoUsageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoUsageWidget)
{
    ui->setupUi(this);
    setAutoFillBackground(true);
    QPalette pal;
    pal.setColor(QPalette::Background, QColor("red"));
    setPalette(pal);
}

QWoUsageWidget::~QWoUsageWidget()
{
    delete ui;
}
