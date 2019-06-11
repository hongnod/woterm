#ifndef QWOHOSTINFOEDIT_H
#define QWOHOSTINFOEDIT_H

#include "qwoglobal.h"

#include <QDialog>

namespace Ui {
class QWoHostInfo;
}

class QWoHostInfoEdit : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostInfoEdit(QWidget *parent = nullptr);
    ~QWoHostInfoEdit();

    HostInfo hostInfo() const;

private slots:
    void onAuthCurrentIndexChanged(const QString & txt);
    void onButtonSaveClicked();    
private:
    Ui::QWoHostInfo *ui;
    HostInfo m_hi;
};

#endif // QWOHOSTINFOEDIT_H
