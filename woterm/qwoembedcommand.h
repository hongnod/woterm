#pragma once

#include <QObject>
#include <QDir>
#include <QPointer>

class QTermWidget;

class QWoEmbedCommand : public QObject
{
    Q_OBJECT
public:
    explicit QWoEmbedCommand(QTermWidget *term, QObject* parent);
    bool cd(const QString& path);
    QString pwd(const QString& path);
private:
    QDir m_current;
    QPointer<QTermWidget> m_term;
};
