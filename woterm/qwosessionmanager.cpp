#include "qwosessionmanager.h"
#include "qwosetting.h"
#include "qwosshconf.h"
#include "qwohostinfoadd.h"
#include "qwohostinfomodify.h"
#include "qwohostinfolist.h"

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
#include <QTimer>
#include <QModelIndexList>
#include <QMenu>
#include <QAction>

QWoSessionManager::QWoSessionManager(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setSpacing(0);
    layout->setMargin(0);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    layout->addLayout(hlayout);
    m_list = new QListView(this);
    m_input = new QLineEdit(this);
    QPushButton *all = new QPushButton("all", this);
    all->setMaximumWidth(25);
    all->hide();
    hlayout->addWidget(m_input);
    hlayout->addWidget(all);
    layout->addWidget(m_list);

    m_list->installEventFilter(this);
    //m_list->setSelectionMode(QAbstractItemView::MultiSelection);

    m_model = new QStringListModel(this);
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    m_list->setModel(m_proxyModel);
    refreshList();

    m_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //m_list->setSelectionMode(QAbstractItemView::MultiSelection);

    QObject::connect(m_input, SIGNAL(returnPressed()), this, SLOT(onEditReturnPressed()));
    QObject::connect(all, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(m_input, SIGNAL(textChanged(const QString&)), this, SLOT(onEditTextChanged(const QString&)));
    QObject::connect(m_list, SIGNAL(clicked()), this, SLOT(onOpenSelectSessions()));
    QObject::connect(m_list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onListItemDoubleClicked(const QModelIndex&)));
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(1000);
    m_countLeft = 0;
}

QWoSessionManager::~QWoSessionManager()
{
    if(m_menu) {
        delete m_menu;
    }
}

void QWoSessionManager::init()
{

}


void QWoSessionManager::refreshList()
{
    QStringList items = QWoSshConf::instance()->hostNameList();
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
    m_countLeft = 30;
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

void QWoSessionManager::onTimeout()
{
    if(m_countLeft < 0) {
        return;
    }
    if(m_countLeft > 0) {
        m_countLeft--;
        return;
    }
    m_input->setText("");
}

void QWoSessionManager::onEditReturnPressed()
{
    QString txt = m_input->text().trimmed();
    if(txt.isEmpty()) {
        return;
    }
    int cnt = m_proxyModel->rowCount();
    if(cnt <= 0) {
        return;
    }
    QModelIndex idx = m_list->currentIndex();
    if(idx.isValid()) {
        idx = m_proxyModel->index(0, 0);
    }
    QString target = idx.data().toString();
    emit sessionDoubleClicked(target);
}

void QWoSessionManager::onListViewItemOpen()
{
    QModelIndex idx = m_list->currentIndex();
    if(idx.isValid()) {
        idx = m_proxyModel->index(0, 0);
    }
    QString target = idx.data().toString();
    emit sessionDoubleClicked(target);
}

void QWoSessionManager::onListViewItemReload()
{
    refreshList();
}

void QWoSessionManager::onListViewItemModify()
{
    QWoHostInfoModify dlg(this);
    dlg.exec();
}

void QWoSessionManager::onListViewItemAdd()
{
    QWoHostInfoAdd dlg(this);
    dlg.exec();
}

void QWoSessionManager::onListViewItemDelete()
{
    QVariant target = m_menu->property("modeIndex");
    if(target.isValid()) {
        QWoSshConf::instance()->remove(target.toString());
        refreshList();
    }
}

bool QWoSessionManager::handleListViewContextMenu(QContextMenuEvent *ev)
{
    QModelIndex mi = m_list->indexAt(ev->pos());
    if(m_menu == nullptr) {
        m_menu = new QMenu();
        m_itemOpen = m_menu->addAction(tr("Open"), this, SLOT(onListViewItemOpen()));
        m_menu->addAction(tr("ReloadAll"), this, SLOT(onListViewItemReload()));
        m_menu->addAction(tr("Edit"), this, SLOT(onListViewItemModify()));
        m_menu->addAction(tr("Add"), this, SLOT(onListViewItemAdd()));
        m_menu->addAction(tr("Delete"), this, SLOT(onListViewItemDelete()));
    }    
    QVariant target = mi.data();
    m_itemOpen->setVisible(mi.isValid());
    if(!mi.isValid()) {
        m_list->clearSelection();
    }
    m_menu->setProperty("modeIndex", target);
    m_menu->exec(ev->globalPos());
    return true;
}


void QWoSessionManager::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QWidget::closeEvent(event);
}

bool QWoSessionManager::eventFilter(QObject *obj, QEvent *ev)
{
    QEvent::Type t = ev->type();
    if(obj == m_list) {
        if(t == QEvent::ContextMenu) {
            return handleListViewContextMenu((QContextMenuEvent *)ev);
        }
    }
    return false;
}
