#ifndef QWOSESSIONMANAGE_H
#define QWOSESSIONMANAGE_H

#include "qwoglobal.h"

#include <QDialog>
#include <QPointer>
#include <QStandardItemModel>

namespace Ui {
class QWoSessionManage;
}

class QTermWidget;
class QStringListModel;


class QWoSessionManage : public QDialog
{
    Q_OBJECT

public:
    explicit QWoSessionManage(QWidget *parent = nullptr);
    ~QWoSessionManage();

signals:
    void connect(const QString& host);

private slots:

private:
    Ui::QWoSessionManage *ui;
    QStandardItemModel m_model;
};

#endif // QWOSESSIONPROPERTY_H
