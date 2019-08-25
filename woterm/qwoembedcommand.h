#pragma once

#include <QObject>
#include <QDir>
#include <QPointer>

class QWoShellWidget;

class QWoEmbedCommand : public QObject
{
    Q_OBJECT
public:
    explicit QWoEmbedCommand(QWoShellWidget *term, QObject* parent);
    bool cd(const QString& path);
    bool pwd();

    QString workPath();
private:
    QDir m_current;
    QPointer<QWoShellWidget> m_term;
};
