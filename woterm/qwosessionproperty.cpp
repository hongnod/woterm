#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"
#include "qwohostsimplelist.h"

#include "qwosetting.h"
#include "qwoutils.h"

#include <QFileDialog>
#include <QDebug>
#include <QIntValidator>
#include <QStringListModel>

QWoSessionProperty::QWoSessionProperty(int type, QWidget *parent)
    : QDialog(parent)
    , m_type(type)
    , ui(new Ui::QWoSessionProperty)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Session Property"));

    ui->tree->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QIntValidator *validator = new QIntValidator();
    validator->setRange(1, 65535);
    ui->port->setValidator(validator);

    ui->fontChooser->setFontFilters(QFontComboBox::MonospacedFonts);

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

    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));

    QObject::connect(ui->connect, SIGNAL(clicked()), this, SLOT(onReadyToConnect()));
    QObject::connect(ui->save, SIGNAL(clicked()), this, SLOT(onReadyToSave()));
    QObject::connect(ui->cancel, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(ui->szDirBrowser, SIGNAL(clicked()), this, SLOT(onSzDirBrowser()));
    QObject::connect(ui->rzDirBrowser, SIGNAL(clicked()), this, SLOT(onRzDirBrowser()));

    QObject::connect(ui->identifyBrowser, SIGNAL(clicked()),  this, SLOT(onIdentifyBrowserClicked()));
    QObject::connect(ui->jumpBrowser, SIGNAL(clicked()),  this, SLOT(onJumpBrowserClicked()));

    initHistory();
    initDefault();


    if(m_type == SPTYPE_DEFAULT) {
        ui->connect->hide();
    }
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}

void QWoSessionProperty::onAuthCurrentIndexChanged(const QString &txt)
{
    bool isPass = txt == tr("Password");

    QWoUtils::setLayoutVisible(ui->passLayout, isPass);
    QWoUtils::setLayoutVisible(ui->identifyLayout, !isPass);
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

void QWoSessionProperty::onIdentifyBrowserClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    qDebug() << "fileName" << fileName;
    fileName = QDir::toNativeSeparators(fileName);
    ui->identify->setCurrentText(fileName);
}

void QWoSessionProperty::onJumpBrowserClicked()
{
    QWoHostSimpleList dlg(this);
    dlg.exec();
    HostInfo hi;
    if(dlg.result(&hi)) {
        ui->jump->setEditText(hi.name);
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
    if(!fileName.isEmpty()) {
        fileName = QDir::toNativeSeparators(fileName);
        ui->szUpload->setText(fileName);
    }
}

void QWoSessionProperty::onRzDirBrowser()
{
    QString path = ui->rzDown->text();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly);
    qDebug() << "fileName" << fileName;
    if(!fileName.isEmpty()) {
        fileName = QDir::toNativeSeparators(fileName);
        ui->rzDown->setText(fileName);
    }
}

void QWoSessionProperty::initDefault()
{
    QDir home = QDir::home();
    QVariant val = QWoSetting::value("property/default");
    QVariantMap mdata = val.toMap();
    QString szPathUpload = mdata.value("zmodemPathUpload", QDir::toNativeSeparators(home.path())).toString();
    QString rzPathDown = mdata.value("zmodemPathDown", QDir::toNativeSeparators(home.path())).toString();
    ui->szUpload->setText(szPathUpload);
    ui->rzDown->setText(rzPathDown);
    ui->port->setText(mdata.value("port", 22).toString());
    QString password = mdata.value("password").toString();
    QString identify = mdata.value("identifyFIle").toString();
    ui->password->setText(password);
    ui->identify->setCurrentText(identify);
    if(!password.isEmpty() || identify.isEmpty()) {
        ui->authType->setCurrentText("Password");
        onAuthCurrentIndexChanged("Password");
    }else if(!identify.isEmpty()){
        ui->authType->setCurrentText("IdentifyFile");
        onAuthCurrentIndexChanged("IdentifyFile");
    }
    QString userName = mdata.value("userName").toString();
    ui->userName->setEditText(userName);
}

void QWoSessionProperty::initHistory()
{
    {
        QVariant v = QWoSetting::value("history/identifyList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->identify->setModel(model);
    }
    {
        QVariant v = QWoSetting::value("history/userNameList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->userName->setModel(model);
    }
    {
        QVariant v = QWoSetting::value("history/proxyJumpList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->jump->setModel(model);
    }
}

void QWoSessionProperty::saveDefaultConfig()
{

}
