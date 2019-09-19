#pragma once

#include <QStackedWidget>
#include <QPointer>

class QTabBar;
class QWoTermWidget;
class QWoTermWidgetImpl;
class QWoShowerWidget;
class QWoUsageWidget;

class QWoShower : public QStackedWidget
{
    Q_OBJECT
private:
    typedef enum {
        ETShell = 0x01,
        ETSsh = 0x02
    } ETabType;
public:
    explicit QWoShower(QTabBar *tab, QWidget *parent=nullptr);
    virtual ~QWoShower();
    bool openLocalShell();
    bool openConnection(const QString& target);
    void setBackgroundColor(const QColor& clr);
    void openFindDialog();

    int tabCount();
signals:
    void tabEmpty();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    void syncGeometry(QWidget *widget);

    void paintEvent(QPaintEvent *event);
    bool event(QEvent* ev);


private:
    void closeSession(int idx);
    void createTab(QWoShowerWidget *widget, const QString& tabName);
    bool tabMouseButtonPress(QMouseEvent *ev);

private slots:
    void onTabCloseRequested(int index);
    void onTabCurrentChanged(int index);
    void onTermImplDestroy(QObject *it);
    void onTabbarDoubleClicked(int index);
private:
    QPointer<QTabBar> m_tabs;
    QFont m_font;
};
