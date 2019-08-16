#include "qwolinenoise.h"
#include "qtermwidget.h"

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

QWoLineNoise::QWoLineNoise(QTermWidget *term, const QByteArray &prompt, QObject *parent)
    : QObject(parent)
    , m_prompt(prompt)
    , m_term(term)
{
    reset();
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
                QByteArray line = m_state.buf;
                reset();
                return line;
            }
            /* Read next character when 0 */
            if (c == 0){
                continue;
            }
        }
        switch(c) {
        case ENTER:    /* enter */
        {
            editMoveEnd();
            QByteArray line = m_state.buf;

            m_term->parseSequenceText("\r\n\x1b[0C");
            handleCommand(line);
            reset();
            m_term->parseSequenceText(m_prompt);
            return line;
        }
        default:
            editInsert(c);
            break;
        }
    }
    return QByteArray();
}

void QWoLineNoise::handleCommand(const QByteArray &buf)
{

}

int QWoLineNoise::termColumn()
{
    return m_term->screenColumnsCount();
}

void QWoLineNoise::reset()
{
    m_state.buf.resize(0);
    m_state.oldpos = m_state.pos = 0;
    m_state.maxrows = m_term->screenLinesCount();
}

char QWoLineNoise::completeLine()
{
    return 0;
}

void QWoLineNoise::editInsert(char c)
{
    if (m_state.buf.length() == m_state.pos) {
        m_state.buf.append(c);
    } else {
        m_state.buf.insert(m_state.pos, c);
    }
    m_state.pos++;
    refreshLine();

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
    char seq[64];
    int plen = m_prompt.length();
    int rows = (plen+m_state.buf.length()+termColumn()-1)/termColumn(); /* rows used by current buf. */
    int rpos = (plen+m_state.oldpos+termColumn())/termColumn(); /* cursor relative row. */
    int old_rows = m_state.maxrows;
    QByteArray ab;

    /* Update maxrows if needed. */
    if (rows > m_state.maxrows) {
        m_state.maxrows = rows;
    }
    if (old_rows-rpos > 0) {
        int n = snprintf(seq,64,"\x1b[%dB", old_rows-rpos);
        ab.append(seq, n);
    }

    /* Now for every row clear it, go up. */
    for (int i = 0; i < old_rows-1; i++) {
        int n = snprintf(seq,64,"\r\x1b[0K\x1b[1A");
        ab.append(seq, n);
    }
    /* Clean the top line. */
    int n = snprintf(seq,64,"\r\x1b[0K");
    ab.append(seq, n);
    /* Write the prompt and the current buffer content */
    ab.append(m_prompt);
    ab.append(m_state.buf);


    /* If we are at the very end of the screen with our prompt, we need to
         * emit a newline and move the prompt to the first column. */
    if(m_state.pos > 0
            && m_state.pos == m_state.buf.length()
            && (m_state.pos + m_prompt.length()) % termColumn() == 0) {
        ab.append('\n');
        n = snprintf(seq, 64, "\r");
        ab.append(seq, n);
        rows++;
        if (rows > m_state.maxrows) {
            m_state.maxrows = rows;
        }
    }
    /* Move cursor to right position. */
    int rpos2 = (m_prompt.length()+m_state.pos+termColumn())/termColumn(); /* current cursor relative row. */
    /* Go up till we reach the expected positon. */
    if (rows-rpos2 > 0) {
        n = snprintf(seq,64,"\x1b[%dA", rows-rpos2);
        ab.append(seq, n);
    }
    /* Set column. */
    int col = (m_prompt.length()+m_state.pos) % termColumn();
    if (col){
        n = snprintf(seq,64,"\r\x1b[%dC", col);
    } else {
        n = snprintf(seq,64,"\r");
    }
    ab.append(seq, n);
    m_state.oldpos = m_state.pos;
    m_term->parseSequenceText(ab);
}
