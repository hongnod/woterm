#include "qwoembedcommand.h"


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
    return m_current.path();
}

QString QWoEmbedCommand::pwd(const QString &path)
{
    return m_current.path();
}
