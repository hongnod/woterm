#include "qwosessionmanager.h"

#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>

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

void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}
