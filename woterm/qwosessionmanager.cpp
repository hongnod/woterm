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

QWoSessionManager::QWoSessionManager(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    m_input = new QLineEdit(this);
    m_list = new QListWidget(this);
    layout->addWidget(m_input);
    layout->addWidget(m_list);
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

void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
