#include "qwoembedcommand.h"
#include "qtermwidget.h"

QWoEmbedCommand::QWoEmbedCommand(QTermWidget *term, QObject *parent)
    :QObject (parent)
    ,m_term(term)
{

}

QString QWoEmbedCommand::cd(const QString &path)
{
    if(!m_current.cd(path)) {
        return m_current.path();
    }
    return QDir::cleanPath(m_current.absolutePath());
}

QString QWoEmbedCommand::pwd()
{
    return QDir::cleanPath(m_current.absolutePath());
}
