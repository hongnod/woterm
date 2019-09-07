#ifndef QWOTERMSTYLE_H
#define QWOTERMSTYLE_H

#include <QProxyStyle>

class QWoTermStyle : public QProxyStyle
{
    Q_OBJECT
public:
    QWoTermStyle();

protected:
    virtual void drawControl(QStyle::ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget = nullptr) const override;
private:

};

#endif // QWOTERMSTYLE_H
