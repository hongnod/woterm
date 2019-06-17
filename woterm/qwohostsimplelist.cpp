#include "qwohostsimplelist.h"
#include "ui_qwohostlist.h"
#include "qwohostlistmodel.h"

#include <QSortFilterProxyModel>

QWoHostSimpleList::QWoHostSimpleList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoHostList)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    m_model = new QWoHostListModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    ui->hostList->setModel(m_proxyModel);

    QObject::connect(ui->rxfind, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));

    m_model->refreshList();
}

QWoHostSimpleList::~QWoHostSimpleList()
{

}

void QWoHostSimpleList::onEditTextChanged(const QString &txt)
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
}
