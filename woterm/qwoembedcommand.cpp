#include "qwoembedcommand.h"
#include "qtermwidget.h"

QWoEmbedCommand::QWoEmbedCommand(QTermWidget *term, QObject *parent)
    :QObject (parent)
    ,m_term(term)
{

}

void QWoEmbedCommand::cd(const QString &path)
{
    if(!m_current.cd(path)) {

    }
}

QString QWoEmbedCommand::workPath()
{
    QDir::cleanPath(m_current.absolutePath());
}

void QWoEmbedCommand::pwd()
{
    QDir::cleanPath(m_current.absolutePath());
}
