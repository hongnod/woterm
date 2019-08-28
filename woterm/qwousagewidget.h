#ifndef QWOUSAGEWIDGET_H
#define QWOUSAGEWIDGET_H

#include <QWidget>

namespace Ui {
class QWoUsageWidget;
}

class QWoUsageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit QWoUsageWidget(QWidget *parent = nullptr);
    ~QWoUsageWidget();

private:
    Ui::QWoUsageWidget *ui;
};

#endif // QWOUSAGEWIDGET_H
