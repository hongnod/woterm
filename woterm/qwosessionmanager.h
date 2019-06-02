#pragma once

#include <QWidget>

#include <QPointer>
class QLineEdit;
class QListWidget;
class QListWidgetItem;

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
private:
    void closeEvent(QCloseEvent *event);
private:
    QPointer<QLineEdit> m_input;
    QPointer<QListWidget> m_list;
    QList<QListWidgetItem> m_items;
};
