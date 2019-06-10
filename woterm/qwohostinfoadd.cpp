#include "qwohostinfoadd.h"
#include "ui_qwohostinfoadd.h"

QWoHostInfoAdd::QWoHostInfoAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoAdd)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    setWindowTitle(tr("Add"));
    ui->setupUi(this);
    ui->passLayout->setEnabled(false);
    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));
}

QWoHostInfoAdd::~QWoHostInfoAdd()
{
    delete ui;
}

void QWoHostInfoAdd::onAuthCurrentIndexChanged(const QString & txt)
{
    bool isPass = txt == tr("Password");

    ui->passLayout->setEnabled(isPass);
    ui->identifyLayout->setEnabled(!isPass);
    ui->vbox->activate();
    ui->vbox->update();
    ui->vbox->invalidate();
}
