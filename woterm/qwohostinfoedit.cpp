#include "qwohostinfoedit.h"
#include "ui_qwohostinfo.h"
#include "qwoutils.h"

#include <QMessageBox>
#include <QPlainTextEdit>

QWoHostInfoEdit::QWoHostInfoEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoHostInfo)
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

QWoHostInfoEdit::~QWoHostInfoEdit()
{
    delete ui;
}

HostInfo QWoHostInfoEdit::hostInfo() const
{
    return m_hi;
}

void QWoHostInfoEdit::onAuthCurrentIndexChanged(const QString & txt)
{
    bool isPass = txt == tr("Password");

    QWoUtils::setLayoutVisible(ui->passLayout, isPass);
    QWoUtils::setLayoutVisible(ui->identifyLayout, !isPass);
}

void QWoHostInfoEdit::onButtonSaveClicked()
{
    m_hi.name = ui->hostName->text();
    m_hi.host = ui->host->text();
    m_hi.port = ui->port->text().toInt();
    m_hi.comment = ui->memo->toPlainText();
    if(ui->authType->currentText() == tr("Password")) {
        m_hi.user = ui->userName->text();
        m_hi.password = ui->password->text();
    }else{
        m_hi.identityFile = ui->identify->text();
    }
    if(m_hi.name.isEmpty()) {
        QMessageBox::warning(this, tr("Info"), tr("The name can't be empty"));
        return;
    }
    if(m_hi.host.isEmpty()) {
        QMessageBox::warning(this, tr("Info"), tr("The host can't be empty"));
        return;
    }
    if(m_hi.port < 10 | m_hi.port > 65535) {
        QMessageBox::warning(this, tr("Info"), tr("The port should be at [10,65535]"));
        return;
    }
    close();
}
