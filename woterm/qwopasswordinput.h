#ifndef QWOPASSWORDINPUT_H
#define QWOPASSWORDINPUT_H

#include "qwowidget.h"

namespace Ui {
class QWoPasswordInput;
}

class QWoPasswordInput : public QWoWidget
{
    Q_OBJECT

public:
    explicit QWoPasswordInput(const QString& prompt, bool echo, QWidget *parent = nullptr);
    ~QWoPasswordInput();

    QString result() const;

private slots:
    void onPasswordVisible(bool checked);
    void onClose();
private:
    void paintEvent(QPaintEvent* paint);
private:
    Ui::QWoPasswordInput *ui;
    QString m_result;
};

#endif // QWOPASSWORDINPUT_H
