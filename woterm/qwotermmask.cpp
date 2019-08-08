#include "qwotermmask.h"
#include "ui_qwotermmask.h"

#include <QPainter>

QWoTermMask::QWoTermMask(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoTermMask)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QObject::connect(ui->btnClose, SIGNAL(click()), this, SLOT(close()));
    QObject::connect(ui->btnReconnect, SIGNAL(click()), this, SIGNAL(reconnect()));
}

QWoTermMask::~QWoTermMask()
{
    delete ui;
}

void QWoTermMask::paintEvent(QPaintEvent *paint)
{
    QPainter p(this);

    p.setBrush(QColor(128,128,128,128));
    p.drawRect(0,0, width(), height());
}
