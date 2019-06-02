#pragma once

#include <QWidget>
#include <QPointer>

class QTabBar;
class QWoTermWidget;

class QWoShower : public QWidget
{
    Q_OBJECT
public:
    explicit QWoShower(QTabBar *tab, QWidget *parent=nullptr);
    virtual ~QWoShower();
    bool openConnection(const QString& target);
protected:
    virtual void resizeEvent(QResizeEvent *event);
    void syncGeometry(QWidget *widget);

private:
    void closeSession(int idx);

private slots:
    void onTabCloseRequested(int index);
    void onTabMoved(int from, int to);
    void onSshProcessFinished(int);
    void onTabCurrentChanged(int index);
private:
    QPointer<QTabBar> m_tabs;
    QList<QPointer<QWoTermWidget>> m_terms;
};
