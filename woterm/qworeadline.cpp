#include "qworeadline.h"

QWoReadLine::QWoReadLine(const QByteArray &prompt, QObject *parent)
    : QObject(parent)
    , m_prompt(prompt)
{

}

void QWoReadLine::append(const QByteArray &buf)
{
    int idx = buf.indexOf('\r');
    if(idx < 0) {
        idx = buf.indexOf('\n');
    }
    if(idx >= 0) {
        m_line.append(buf.left(idx));
        handleCommand(m_line);
        m_line = "\n"+m_prompt;
        return;
    }
    m_line.append(buf);
}

void QWoReadLine::handleCommand(const QByteArray &cmd)
{
    QByteArray line = cmd.trimmed();
    QByteArray echo="\n";
    echo.append(cmd);
    emit result(echo);
}
