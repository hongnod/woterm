#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"

#include "qwosetting.h"

#include <QFileDialog>
#include <QDebug>

QWoSessionProperty::QWoSessionProperty(int type, QWidget *parent)
    : QDialog(parent)
    , m_type(type)
    , ui(new Ui::QWoSessionProperty)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Session Property"));

    ui->tree->setModel(&m_model);
    ui->tree->setIndentation(10);
    QStandardItem *connect = new QStandardItem(tr("Connect"));
    m_model.appendRow(connect);
    connect->appendRow(new QStandardItem(tr("Authentication")));

    QStandardItem *terminal = new QStandardItem(tr("Terminal"));
    m_model.appendRow(terminal);


    QStandardItem *appearance = new QStandardItem(tr("Appearance"));
    m_model.appendRow(appearance);

    QStandardItem *fileTransfre = new QStandardItem(tr("FileTransfer"));
    m_model.appendRow(fileTransfre);


    QObject::connect(ui->tree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeItemClicked(const QModelIndex&)));

    QObject::connect(ui->connect, SIGNAL(clicked()), this, SLOT(onReadyToConnect()));
    QObject::connect(ui->save, SIGNAL(clicked()), this, SLOT(onReadyToSave()));
    QObject::connect(ui->cancel, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(ui->szDirBrowser, SIGNAL(clicked()), this, SLOT(onSzDirBrowser()));
    QObject::connect(ui->rzDirBrowser, SIGNAL(clicked()), this, SLOT(onRzDirBrowser()));

    initDefault();

    if(m_type == SPTYPE_DEFAULT) {
        ui->connect->hide();
    }
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}

void QWoSessionProperty::onTreeItemClicked(const QModelIndex &idx)
{
    QString name = idx.data().toString();
    if(name == tr("Connect")) {
        ui->stacked->setCurrentWidget(ui->connectWidget);
    }else if(name == tr("Authentication")){
        ui->stacked->setCurrentWidget(ui->authenticationWidget);
    }else if(name == tr("Terminal")){
        ui->stacked->setCurrentWidget(ui->terminalWidget);
    }else if(name == tr("Appearance")){
        ui->stacked->setCurrentWidget(ui->appearanceWidget);
    }else if(name == tr("FileTransfer")){
        ui->stacked->setCurrentWidget(ui->fileTransferWidget);
    }
}

void QWoSessionProperty::onReadyToConnect()
{

}

void QWoSessionProperty::onReadyToSave()
{

}

void QWoSessionProperty::onSzDirBrowser()
{
    QString path = ui->szUpload->text();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly);
    qDebug() << "fileName" << fileName;
    fileName = QDir::toNativeSeparators(fileName);
    ui->szUpload->setText(fileName);
}

void QWoSessionProperty::onRzDirBrowser()
{
    QString path = ui->rzDown->text();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly);
    qDebug() << "fileName" << fileName;
    fileName = QDir::toNativeSeparators(fileName);
    ui->rzDown->setText(fileName);
}

void QWoSessionProperty::initDefault()
{
    QVariant val = QWoSetting::value("property/default");
    QVariantMap mdata = val.toMap();
    QString szPathUpload = mdata["zmodemPathUpload"].toString();
    QString rzPathDown = mdata["zmodemPathDown"].toString();
    ui->szUpload->setText(szPathUpload);
    ui->rzDown->setText(rzPathDown);
}

void QWoSessionProperty::saveDefaultConfig()
{

}
