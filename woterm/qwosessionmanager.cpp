#include "qwosessionmanager.h"
#include "qwosetting.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QApplication>
#include <QFile>
#include <QRegExp>

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
    QString content = file.readAll();
    QRegExp rx("^Host ");
    int pos = 0;
    while ((pos = rx.indexIn(content, pos)) != -1) {
        pos += rx.matchedLength();
    }
}

void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
