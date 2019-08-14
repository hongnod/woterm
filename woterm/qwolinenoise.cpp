#include "qwolinenoise.h"

enum KEY_ACTION{
    KEY_NULL = 0,	    /* NULL */
    CTRL_A = 1,         /* Ctrl+a */
    CTRL_B = 2,         /* Ctrl-b */
    CTRL_C = 3,         /* Ctrl-c */
    CTRL_D = 4,         /* Ctrl-d */
    CTRL_E = 5,         /* Ctrl-e */
    CTRL_F = 6,         /* Ctrl-f */
    CTRL_H = 8,         /* Ctrl-h */
    TAB = 9,            /* Tab */
    CTRL_K = 11,        /* Ctrl+k */
    CTRL_L = 12,        /* Ctrl+l */
    ENTER = 13,         /* Enter */
    CTRL_N = 14,        /* Ctrl-n */
    CTRL_P = 16,        /* Ctrl-p */
    CTRL_T = 20,        /* Ctrl-t */
    CTRL_U = 21,        /* Ctrl+u */
    CTRL_W = 23,        /* Ctrl+w */
    ESC = 27,           /* Escape */
    BACKSPACE =  127    /* Backspace */
};

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

QByteArray QWoLineNoise::parse(const QByteArray &buf)
{
    for(int i = 0; i < buf.size(); i++) {
        char seq[3];
        char c = buf.at(i);
        if(c == 9) {
            c = completeLine();
            /* Return on errors */
            if (c < 0) {
                //return l.len;
                return m_state.buf;
            }
            /* Read next character when 0 */
            if (c == 0){
                continue;
            }
        }
        switch(c) {
        case ENTER:    /* enter */

            break;
        }
    }
    return QByteArray();
}

void QWoLineNoise::setColumn(int n)
{

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

void QWoLineNoise::reset()
{
    m_state.buf.resize(0);
    m_state.oldpos = m_state.pos = m_state.maxrows = 0;
}

char QWoLineNoise::completeLine()
{
    return 0;
}

void QWoLineNoise::refreshLine()
{
    refreshMultiLine();
}

/* Move cursor to the end of the line. */
void QWoLineNoise::editMoveEnd()
{
    if (m_state.pos != m_state.buf.length()) {
        m_state.pos = m_state.buf.length();
        refreshLine();
    }
}

void QWoLineNoise::refreshMultiLine()
{

}
