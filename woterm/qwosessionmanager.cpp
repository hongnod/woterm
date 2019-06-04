#include "qwosessionmanager.h"
#include "qwosetting.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QApplication>
#include <QFile>
#include <QRegExp>
#include <QDebug>
#include <QtAlgorithms>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QStringListModel>

QWoSessionManager::QWoSessionManager(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    layout->addLayout(hlayout);
    QListView *list = new QListView(this);
    QLineEdit *input = new QLineEdit(this);
    QPushButton *reload = new QPushButton("load", this);
    QPushButton *all = new QPushButton("all", this);
    hlayout->addWidget(reload);
    hlayout->addWidget(input);
    hlayout->addWidget(all);
    layout->addWidget(list);

    QObject::connect(reload, SIGNAL(clicked()), this, SLOT(onReloadSessionList()));
    QObject::connect(all, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(input, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));

    m_model = new QStringListModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    list->setModel(m_proxyModel);
    refreshList();

    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    list->setSelectionMode(QAbstractItemView::MultiSelection);
    //QString regExp = QString("^[%1].*").arg(str);
    //proxy->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));
}

void QWoSessionManager::init()
{

}



void QWoSessionManager::refreshList()
{

    QString cfg = QWoSetting::sshServerListPath();
    if(cfg.isEmpty()) {
        return;
    }
    QFile file(cfg);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }
    QStringList items;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        line = line.trimmed();
        if(!line.startsWith("Host ")) {
            continue;
        }
        QList<QByteArray> sets = line.split(' ');
        for(int i = 0; i < sets.length(); i++) {
            QByteArray name = sets.at(i).trimmed();
            if(name.contains('*')) {
                continue;
            }
            if(name == "Host") {
                continue;
            }
            items.append(name);
        }
    }
    m_model->setStringList(items);
}

void QWoSessionManager::onReloadSessionList()
{
    refreshList();
}

void QWoSessionManager::onOpenSelectSessions()
{

}

void QWoSessionManager::onEditTextChanged(const QString &txt)
{
    m_proxyModel->setFilterRegExp(txt);
}


void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
