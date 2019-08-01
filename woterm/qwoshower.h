#pragma once

#include <QStackedWidget>
#include <QPointer>

class QTabBar;
class QWoTermWidget;
class QWoTermWidgetImpl;

class QWoShower : public QStackedWidget
{
    Q_OBJECT
public:
    explicit QWoShower(QTabBar *tab, QWidget *parent=nullptr);
    virtual ~QWoShower();
    bool openConnection(const QString& target);
    void setBackgroundColor(const QColor& clr);
    void openFindDialog();
protected:
    virtual void resizeEvent(QResizeEvent *event);
    void syncGeometry(QWidget *widget);

private:
    void closeSession(int idx);

private slots:
    void onTabCloseRequested(int index);
    void onTabCurrentChanged(int index);
    void onTermImplDestroy(QObject *it);
private:
    QPointer<QTabBar> m_tabs;
};
