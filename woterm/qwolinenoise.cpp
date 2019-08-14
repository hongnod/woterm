#include "qwolinenoise.h"

QWoLineNoise::QWoLineNoise(const QByteArray &prompt, QObject *parent)
    : QObject(parent)
    , m_prompt("\x1b[0K"+prompt)
{

}

QByteArray QWoLineNoise::append(const QByteArray &buf)
{
    int idx = buf.indexOf('\r');
    if(idx < 0) {
        idx = buf.indexOf('\n');
    }
    if(idx >= 0) {
        m_line.append(buf.left(idx));
        QByteArray result = handleCommand(m_line);
        m_line = "\n"+m_prompt;
        result.append(m_line);
        return handleResult(result);
    }
    m_line.append(buf);
    return handleResult(m_line);
}

QByteArray QWoLineNoise::handleCommand(const QByteArray &cmd)
{
    QByteArray line = cmd.trimmed();
    QByteArray echo="\n";
    echo.append(cmd);
    return echo;
}

QByteArray QWoLineNoise::handleResult(const QByteArray& data)
{
    QByteArray result;
    //result.append('\r');
    result.append(data);
    result.append("\x1b[0K");
    //result.append("\r\x1b[%dC");
    result.append("\r\x1b[1C");
    return result;
}
