#ifndef QWOSESSIONPROPERTY_H
#define QWOSESSIONPROPERTY_H

#include "qwoglobal.h"

#include <QDialog>
#include <QPointer>
#include <QStandardItemModel>

namespace Ui {
class QWoSessionProperty;
}

class QTermWidget;
class QStringListModel;


class QWoSessionProperty : public QDialog
{
    Q_OBJECT

public:
    explicit QWoSessionProperty(int idx, QWidget *parent = nullptr);
    ~QWoSessionProperty();

signals:
    void connect(const QString& host);

private slots:
    void onAuthCurrentIndexChanged(const QString & txt);
    void onColorCurrentIndexChanged(const QString & txt);
    void onKeyBindCurrentIndexChanged(const QString & txt);
    void onCurrentFontChanged(const QFont &font);

    void onBlockCursorToggled();
    void onUnderlineCursorToggled();
    void onBeamCursorToggled();

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
    void initCustom();
    void resetProerty(QVariantMap mdata);
    void saveConfig();
    void saveHistory();
    void setFixPreviewString();

private slots:
    void onTimeout();

private:
    const int m_idx;
    Ui::QWoSessionProperty *ui;
    QStandardItemModel m_model;
    QPointer<QTermWidget> m_preview;
};

#endif // QWOSESSIONPROPERTY_H
