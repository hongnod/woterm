#pragma once

#include <QWidget>

#include <QPointer>
class QLineEdit;
class QListWidget;
class QListWidgetItem;
class QPushButton;

class QWoSessionManager : public QWidget
{
    Q_OBJECT
public:
    explicit QWoSessionManager(QWidget *parent=nullptr);
signals:
    void aboutToClose(QCloseEvent* event);
private:
    void init();
    void refreshList();
    int findIndex(const QString& name);
private slots:
    void onReloadSessionList();
    void onOpenSelectSessions();
    void onEditTextChanged(const QString& txt);
private:
    void closeEvent(QCloseEvent *event);
private:
    QPointer<QListWidget> m_list;
    QList<QListWidgetItem*> m_items;
};
