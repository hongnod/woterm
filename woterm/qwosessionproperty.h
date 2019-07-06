#ifndef QWOSESSIONPROPERTY_H
#define QWOSESSIONPROPERTY_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class QWoSessionProperty;
}

class QWoSessionProperty : public QDialog
{
    Q_OBJECT

public:
    explicit QWoSessionProperty(QWidget *parent = nullptr);
    ~QWoSessionProperty();

private slots:
    void onTreeItemClicked(const QModelIndex& index);

private:
    Ui::QWoSessionProperty *ui;
    QStandardItemModel m_model;
};

#endif // QWOSESSIONPROPERTY_H
