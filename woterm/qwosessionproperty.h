#ifndef QWOSESSIONPROPERTY_H
#define QWOSESSIONPROPERTY_H

#include <QDialog>
#include <QPointer>
#include <QStandardItemModel>

namespace Ui {
class QWoSessionProperty;
}

class QTermWidget;
class QStringListModel;

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
    void onColorCurrentIndexChanged(const QString & txt);
    void onCurrentFontChanged(const QFont &font);

    void onBlockCursorToggled(bool checked);
    void onUnderlineCursorToggled(bool checked);
    void onBeamCursorToggled(bool checked);

    void onFontValueChanged(int i);

    void onTreeItemClicked(const QModelIndex& index);
    void onIdentifyBrowserClicked();
    void onJumpBrowserClicked();
    void onReadyToConnect();
    void onReadyToSave();
    void onSzDirBrowser();
    void onRzDirBrowser();


private:
    void initDefault();
    void initHistory();
    void saveDefaultConfig();
private:
    const int m_type;
    Ui::QWoSessionProperty *ui;
    QStandardItemModel m_model;
    QPointer<QStringListModel> m_modelPreview;
    QPointer<QTermWidget> m_preview;
};

#endif // QWOSESSIONPROPERTY_H
