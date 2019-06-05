#pragma once

#include <QWidget>

#include <QPointer>
class QLineEdit;
class QListWidget;
class QPushButton;
class QStringListModel;
class QSortFilterProxyModel;
class QListView;

class QWoSessionManager : public QWidget
{
    Q_OBJECT
public:
    explicit QWoSessionManager(QWidget *parent=nullptr);
signals:
    void aboutToClose(QCloseEvent* event);
    void sessionDoubleClicked(const QString& session);
    void sessionBatchClicked(const QStringList& sessions);
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
private:
    void closeEvent(QCloseEvent *event);
private:
    QPointer<QStringListModel> m_model;
    QPointer<QSortFilterProxyModel> m_proxyModel;
    QPointer<QLineEdit> m_input;
    QPointer<QListView> m_list;
    int m_countLeft;
};
