#ifndef QWOHOSTINFOADD_H
#define QWOHOSTINFOADD_H

#include "qwoglobal.h"

#include <QDialog>

namespace Ui {
class QWoHostInfoAdd;
}

class QWoHostInfoAdd : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostInfoAdd(QWidget *parent = nullptr);
    ~QWoHostInfoAdd();

    HostInfo hostInfo() const;

private slots:
    void onAuthCurrentIndexChanged(const QString & txt);
    void onButtonSaveClicked();    
private:
    Ui::QWoHostInfoAdd *ui;
    HostInfo m_hi;
};

#endif // QWOHOSTINFOADD_H
