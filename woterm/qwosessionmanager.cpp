#include "qwosessionmanager.h"
#include "qwosetting.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QApplication>
#include <QFile>
#include <QRegExp>
#include <QListWidgetItem>
#include <QDebug>
#include <QtAlgorithms>
#include <QPushButton>

QWoSessionManager::QWoSessionManager(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    layout->addLayout(hlayout);
    m_list = new QListWidget(this);
    QLineEdit *input = new QLineEdit(this);
    QPushButton *reload = new QPushButton("load", this);
    QPushButton *all = new QPushButton("all", this);
    hlayout->addWidget(reload);
    hlayout->addWidget(input);
    hlayout->addWidget(all);
    layout->addWidget(m_list);

    QObject::connect(reload, SIGNAL(clicked()), this, SLOT(onReloadSessionList()));
    QObject::connect(all, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(input, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));
}

void QWoSessionManager::init()
{

}


bool lessThan(QListWidgetItem* a, QListWidgetItem *b)
{
    return a->text() < b->text();
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
    QList<QListWidgetItem*> items;
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
            int idx = findIndex(name);
            if(idx >= 0) {
                items.append(m_items.takeAt(idx));
            }else{
                QListWidgetItem *item = new QListWidgetItem(name, m_list);
                items.append(item);
            }
        }
    }
    m_items.swap(items);
    while(items.length() > 0){
        delete items.takeFirst();
    }
    std::sort(m_items.begin(), m_items.end(), lessThan);

    m_list.clear();
    for(int i = 0; i < m_items.length(); i++) {
        m_list->addItem(m_items.at(i));
    }

}

int QWoSessionManager::findIndex(const QString &name)
{
    for(int i = 0; i < m_items.length(); i++) {
        QListWidgetItem* item = m_items.at(i);
        QString txt = item->text();
        if(txt == name) {
            return i;
        }
    }
    return -1;
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

}


void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
