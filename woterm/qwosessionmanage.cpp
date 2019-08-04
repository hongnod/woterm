#include "qwosessionmanage.h"
#include "ui_qwosessionmanage.h"
#include "qwohostsimplelist.h"
#include "qtermwidget.h"

#include "qwosetting.h"
#include "qwoutils.h"
#include "qwosshconf.h"

#include <QFileDialog>
#include <QDebug>
#include <QIntValidator>
#include <QStringListModel>
#include <QMessageBox>
#include <QSortFilterProxyModel>

QWoSessionManage::QWoSessionManage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoSessionManage)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Session List"));

    m_model = new QWoHostListModel(this);
    m_model->setMaxColumnCount(3);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    ui->treeView->setModel(m_proxyModel);
    ui->treeView->setColumnWidth(0, 100);
    ui->treeView->setColumnWidth(1, 200);

    QObject::connect(ui->lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));
    QObject::connect(ui->btnConnect, SIGNAL(click()), this, SLOT(onConnectReady()));
    QObject::connect(ui->btnDelete, SIGNAL(click()), this, SLOT(onDeleteReady()));
    QObject::connect(ui->btnModify, SIGNAL(click()), this, SLOT(onModifyReady()));
    QObject::connect(ui->btnNew, SIGNAL(click()), this, SLOT(onNewReady()));
}

QWoSessionManage::~QWoSessionManage()
{
    delete ui;
}

void QWoSessionManage::onEditTextChanged(const QString &txt)
{
    QStringList sets = txt.split(' ');
    for(QStringList::iterator iter = sets.begin(); iter != sets.end(); ) {
        if(*iter == "") {
            iter = sets.erase(iter);
        }else{
            iter++;
        }
    }

    QRegExp regex(sets.join(".*"), Qt::CaseInsensitive);
    regex.setPatternSyntax(QRegExp::RegExp2);
    m_proxyModel->setFilterRegExp(regex);
    m_proxyModel->setFilterRole(ROLE_REFILTER);
}
