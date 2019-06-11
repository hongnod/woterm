#include "qwohostinfoadd.h"
#include "ui_qwohostinfoadd.h"
#include "qwoutils.h"

QWoHostInfoAdd::QWoHostInfoAdd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfoAdd)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Add"));
    onAuthCurrentIndexChanged(ui->authType->currentText());
    QIntValidator *validator = new QIntValidator(10, 65535, ui->port);
    ui->port->setValidator(validator);
    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));
    QObject::connect(ui->save, SIGNAL(clicked()),  this, SLOT(onButtonSaveClicked()));
}

QWoHostInfoAdd::~QWoHostInfoAdd()
{
    delete ui;
}

void QWoHostInfoAdd::onAuthCurrentIndexChanged(const QString & txt)
{
    bool isPass = txt == tr("Password");

    QWoUtils::setLayoutVisible(ui->passLayout, isPass);
    QWoUtils::setLayoutVisible(ui->identifyLayout, !isPass);
}

void QWoHostInfoAdd::onButtonSaveClicked()
{
    QString hostName = ui->hostName->text();
    QString host = ui->host->text();
    int port = ui->port->text().toInt();
    QString
}
