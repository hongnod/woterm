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
    void cd(const QString& path);
    QString workPath();
    void pwd();
private:
    QDir m_current;
    QPointer<QTermWidget> m_term;
};
