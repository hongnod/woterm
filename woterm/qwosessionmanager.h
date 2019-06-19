#pragma once

#include <QWidget>
#include <QPointer>
#include "qwoglobal.h"

class QLineEdit;
class QListWidget;
class QPushButton;
class QWoHostListModel;
class QSortFilterProxyModel;
class QListView;
class QMenu;


class QWoSessionManager : public QWidget
{
    Q_OBJECT
public:
    explicit QWoSessionManager(QWidget *parent=nullptr);
    virtual ~QWoSessionManager();
signals:
    void aboutToClose(QCloseEvent* event);
    void sessionDoubleClicked(const HostInfo& hi);
    void sessionBatchClicked(const QVariantList& his);
private:
    void init();
    void refreshList();
private slots:
    void onReloadSessionList();
    void onOpenSelectSessions();
    void onEditTextChanged(const QString& txt);
    void onListItemDoubleClicked(const QModelIndex& item);
    void onTimeout();
    void onEditReturnPressed();
    void onListViewItemOpen();
    void onListViewItemReload();
    void onListViewItemModify();
    void onListViewItemAdd();
    void onListViewItemDelete();
private:
    bool handleListViewContextMenu(QContextMenuEvent* ev);
private:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    QPointer<QWoHostListModel> m_model;
    QPointer<QSortFilterProxyModel> m_proxyModel;
    QPointer<QLineEdit> m_input;
    QPointer<QListView> m_list;
    int m_countLeft;
    QPointer<QMenu> m_menu;
    QPointer<QAction> m_itemOpen;
};
