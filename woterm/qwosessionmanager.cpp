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
    QPushButton *all = new QPushButton("all", this);
    all->setMaximumWidth(25);
    all->hide();
    hlayout->addWidget(input);
    hlayout->addWidget(all);
    layout->addWidget(list);

    m_model = new QStringListModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    list->setModel(m_proxyModel);
    refreshList();

    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //list->setSelectionMode(QAbstractItemView::MultiSelection);

    QObject::connect(all, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(input, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));
    QObject::connect(list, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onListItemDoubleClicked(const QModelIndex&)));
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
    int cnt = m_proxyModel->rowCount();
    qDebug() << "rowCount:" << cnt;
    QStringList sessions;
    for(int i = 0; i < cnt; i++) {
        QModelIndex mi = m_proxyModel->index(i, 0);
        QString name = mi.data().toString();
        qDebug() << "name:" << name;
        sessions << name;
    }
    emit sessionBatchClicked(sessions);
}

void QWoSessionManager::onEditTextChanged(const QString &txt)
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

void QWoSessionManager::onListItemDoubleClicked(const QModelIndex &item)
{
    QString name = item.data().toString();
    if(name == "") {
        return;
    }
    qDebug() << "server:" << name;
    emit sessionDoubleClicked(name);
}


void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
