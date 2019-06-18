#include "qwohostinfoedit.h"
#include "ui_qwohostinfo.h"
#include "qwoutils.h"
#include "qwosshconf.h"
#include "qwohostsimplelist.h"

#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QDebug>

QWoHostInfoEdit::QWoHostInfoEdit(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoHostInfo)
    , m_idx(-1)
{
    init();
    setWindowTitle(tr("Add"));
}

QWoHostInfoEdit::QWoHostInfoEdit(int idx, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoHostInfo)
    , m_idx(idx)
{
    init();
    setWindowTitle(tr("Modify"));
    HostInfo hi = QWoSshConf::instance()->hostInfo(m_idx);
    ui->hostName->setText(hi.name);
    ui->host->setText(hi.host);
    ui->port->setText(QString("%1").arg(hi.port));
    ui->memo->setPlainText(hi.memo);
    ui->userName->setText(hi.user);
    ui->password->setText(hi.password);
    ui->identify->setText(hi.identityFile);
    if(!hi.password.isEmpty()) {
        ui->authType->setCurrentText("Password");
    }else{
        ui->authType->setCurrentText("IdentifyFile");
    }
    ui->jump->setText(hi.proxyJump);
}

QWoHostInfoEdit::~QWoHostInfoEdit()
{
    delete ui;
}

void QWoHostInfoEdit::onAuthCurrentIndexChanged(const QString & txt)
{
    bool isPass = txt == tr("Password");

    QWoUtils::setLayoutVisible(ui->passLayout, isPass);
    QWoUtils::setLayoutVisible(ui->identifyLayout, !isPass);
}

void QWoHostInfoEdit::onButtonSaveClicked()
{
    HostInfo hi;
    hi.name = ui->hostName->text();
    hi.host = ui->host->text();
    hi.port = ui->port->text().toInt();
    hi.memo = ui->memo->toPlainText();
    hi.user = ui->userName->text();
    hi.password = ui->password->text();
    hi.identityFile = ui->identify->text();
    hi.proxyJump = ui->jump->text();

    if(hi.name.isEmpty()) {
        QMessageBox::warning(this, tr("Info"), tr("The name can't be empty"));
        return;
    }
    if(hi.host.isEmpty()) {
        QMessageBox::warning(this, tr("Info"), tr("The host can't be empty"));
        return;
    }
    if(hi.port < 10 | hi.port > 65535) {
        QMessageBox::warning(this, tr("Info"), tr("The port should be at [10,65535]"));
        return;
    }
    close();
    if(m_idx < 0) {
        QWoSshConf::instance()->append(hi);
        return;
    }
    QWoSshConf::instance()->modify(m_idx, hi);
}

void QWoHostInfoEdit::onButtonJumpBrowserClicked()
{
    QWoHostSimpleList dlg(this);
    dlg.exec();
}

void QWoHostInfoEdit::onButtonIdentifyBrowserClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    qDebug() << "fileName" << fileName;
    ui->identify->setText(fileName);
}

void QWoHostInfoEdit::init()
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Add"));
    onAuthCurrentIndexChanged(ui->authType->currentText());
    QIntValidator *validator = new QIntValidator(10, 65535, ui->port);
    ui->port->setValidator(validator);
    ui->port->setText("22");
    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));
    QObject::connect(ui->save, SIGNAL(clicked()),  this, SLOT(onButtonSaveClicked()));
    QObject::connect(ui->identifyBrowser, SIGNAL(clicked()),  this, SLOT(onButtonIdentifyBrowserClicked()));
    QObject::connect(ui->jumpBrowser, SIGNAL(clicked()),  this, SLOT(onButtonJumpBrowserClicked()), Qt::QueuedConnection);
}
