#include "qwoembedcommand.h"
#include "qwoshellwidget.h"

QWoEmbedCommand::QWoEmbedCommand(QWoShellWidget *term, QObject *parent)
    :QObject (parent)
    ,m_term(term)
{

}

bool QWoEmbedCommand::cd(const QString &path)
{
    if(!m_current.cd(path)) {
        m_term->parseSequenceText(QString("No such path:"+path+"\r\n").toUtf8());
        return false;
    }
    return true;
}

QString QWoEmbedCommand::workPath()
{
    return QDir::toNativeSeparators(QDir::cleanPath(m_current.absolutePath()));
}

bool QWoEmbedCommand::pwd()
{
    QString path = QDir::toNativeSeparators(QDir::cleanPath(m_current.absolutePath()));
    m_term->parseSequenceText(QString(path+"\r\n").toUtf8());
    return true;
}
