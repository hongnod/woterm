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
    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));
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
