#ifndef QWOHOSTEDITDIALOG_H
#define QWOHOSTEDITDIALOG_H

#include <QDialog>

namespace Ui {
class QWoHostEditDialog;
}

class QWoHostEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QWoHostEditDialog(QWidget *parent = nullptr);
    ~QWoHostEditDialog();

private:
    Ui::QWoHostEditDialog *ui;
};

#endif // QWOHOSTEDITDIALOG_H
