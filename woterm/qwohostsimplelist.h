#ifndef QWOHOSTSIMPLELIST_H
#define QWOHOSTSIMPLELIST_H

#include "qwoglobal.h"

#include <QDialog>
#include <QPointer>

namespace Ui {
class QWoHostList;
}

class QWoHostListModel;
class QSortFilterProxyModel;

class QWoHostSimpleList : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostSimpleList(QWidget *parent = nullptr);
    ~QWoHostSimpleList();

private slots:
    void onEditTextChanged(const QString& txt);
private:
    void init();
private:
    Ui::QWoHostList *ui;
    QPointer<QWoHostListModel> m_model;
    QPointer<QSortFilterProxyModel> m_proxyModel;
};

#endif // QWOHOSTSIMPLELIST_H
