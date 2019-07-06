#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"

QWoSessionProperty::QWoSessionProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoSessionProperty)
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
