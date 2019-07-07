#ifndef QWOSESSIONPROPERTY_H
#define QWOSESSIONPROPERTY_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class QWoSessionProperty;
}

#define SPTYPE_DEFAULT    (1)
#define SPTYPE_MODIFY     (2)
#define SPTYPE_NEW        (3)

class QWoSessionProperty : public QDialog
{
    Q_OBJECT

public:
    explicit QWoSessionProperty(int type, QWidget *parent = nullptr);
    ~QWoSessionProperty();

private slots:
    void onAuthCurrentIndexChanged(const QString & txt);
    void onTreeItemClicked(const QModelIndex& index);
    void onReadyToConnect();
    void onReadyToSave();
    void onSzDirBrowser();
    void onRzDirBrowser();


private:
    void initDefault();
    void saveDefaultConfig();
private:
    const int m_type;
    Ui::QWoSessionProperty *ui;
    QStandardItemModel m_model;
};

#endif // QWOSESSIONPROPERTY_H
