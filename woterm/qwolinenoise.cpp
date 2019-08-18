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
    BACKSPACE = 8,    /* Backspace */
    DELETE = 127           /* delete */
};

QWoLineNoise::QWoLineNoise(QTermWidget *term)
    : m_prompt("$")
    , m_term(term)
{
    reset();
}

void QWoLineNoise::parse(const QByteArray &buf)
{
    int iread = 0;
    while(iread < buf.count()) {
        char seq[3];
        char c = buf.at(iread++);
        if(c == 9) {
            c = completeLine();
            /* Return on errors */
            if (c < 0) {
                QByteArray line = m_state.buf;
                reset();
                handleCommand(line);
                return;
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
            m_term->parseSequenceText("\r\n");
            handleCommand(line);
            reset();
            m_term->parseSequenceText(m_prompt);
            return;
        }
        case CTRL_C:     /* ctrl-c */
            m_term->parseSequenceText("^C\r\n");
            reset();
            m_term->parseSequenceText(m_prompt);
            return ;
        case BACKSPACE:   /* backspace */
            editBackspace();
            break;
        case DELETE:     /* ctrl-h */
            editDelete();
            break;
        case CTRL_D:     /* ctrl-d, remove char at right of cursor, or if the
                            line is empty, act as end-of-file. */
            if (m_state.buf.length() > 0) {
                editDelete();
            }
            break;
        case CTRL_B:     /* ctrl-b */
            editMoveLeft();
            break;
        case CTRL_F:     /* ctrl-f */
            editMoveRight();
            break;
        case CTRL_P:    /* ctrl-p */
            editHistoryPrev();
            break;
        case CTRL_N:    /* ctrl-n */
            editHistoryNext();
            break;
        case ESC:    /* escape sequence */
            /* Read the next two bytes representing the escape sequence.
             * Use two calls to handle slow terminals returning the two
             * chars at different times. */
            seq[0] = buf[iread++];
            seq[1] = buf[iread++];

            /* ESC [ sequences. */
            if (seq[0] == '[') {
                if (seq[1] >= '0' && seq[1] <= '9') {
                    /* Extended escape, read additional byte. */
                    seq[2] = buf[iread++];
                    if (seq[2] == '~') {
                        switch(seq[1]) {
                        case '3': /* Delete key. */
                            editDelete();
                            break;
                        }
                    }
                } else {
                    switch(seq[1]) {
                    case 'A': /* Up */
                        editHistoryPrev();
                        break;
                    case 'B': /* Down */
                        editHistoryNext();
                        break;
                    case 'C': /* Right */
                        editMoveRight();
                        break;
                    case 'D': /* Left */
                        editMoveLeft();
                        break;
                    case 'H': /* Home */
                        editMoveHome();
                        break;
                    case 'F': /* End*/
                        editMoveEnd();
                        break;
                    }
                }
            } else if (seq[0] == 'O') {
                /* ESC O sequences. */
                switch(seq[1]) {
                case 'H': /* Home */
                    editMoveHome();
                    break;
                case 'F': /* End*/
                    editMoveEnd();
                    break;
                }
            }
            break;
        case CTRL_U: /* Ctrl+u, delete the whole line. */
            reset();
            refreshLine();
            break;
        case CTRL_K: /* Ctrl+k, delete from current to end of line. */
            m_state.buf = m_state.buf.left(m_state.pos);
            refreshLine();
            break;
        case CTRL_A: /* Ctrl+a, go to the start of the line */
            editMoveHome();
            break;
        case CTRL_E: /* ctrl+e, go to the end of the line */
            editMoveEnd();
            break;
        case CTRL_L: /* ctrl+l, clear screen */
            clearScreen();
            refreshLine();
            break;
        default:
            if(!QChar::fromLatin1(c).isPrint()){
                return;
            }
            editInsert(c);
            break;
        }

    }
}

void QWoLineNoise::setPrompt(const QByteArray &prompt)
{
    m_prompt = prompt;
}

int QWoLineNoise::column()
{
    return m_term->screenColumnsCount();
}

void QWoLineNoise::editDelete()
{
    if (m_state.pos < m_state.buf.length()) {
         m_state.buf.remove(m_state.pos, 1);
         refreshLine();
    }
}

void QWoLineNoise::editHistoryPrev()
{

}

void QWoLineNoise::editHistoryNext()
{

}


void QWoLineNoise::editMoveRight()
{
    if (m_state.pos != m_state.buf.length()) {
        m_state.pos++;
        refreshLine();
    }
}

void QWoLineNoise::editMoveLeft()
{
    if (m_state.pos > 0) {
        m_state.pos--;
        refreshLine();
    }
}

void QWoLineNoise::editMoveHome()
{
    if (m_state.pos != 0) {
        m_state.pos = 0;
        refreshLine();
    }
}

void QWoLineNoise::editBackspace()
{
    if (m_state.pos > 0 && !m_state.buf.isEmpty()) {
        m_state.buf.remove(m_state.pos-1, 1);
        m_state.pos--;
        refreshLine();
    }
}

void QWoLineNoise::clearScreen()
{
    m_term->parseSequenceText("\x1b[H\x1b[2J");
}

void QWoLineNoise::editDeletePrevWord()
{

}

/* Move cursor to the end of the line. */
void QWoLineNoise::editMoveEnd()
{
    if (m_state.pos != m_state.buf.length()) {
        m_state.pos = m_state.buf.length();
        refreshLine();
    }
}

void QWoLineNoise::reset()
{
    m_state.buf.resize(0);
    m_state.oldpos = m_state.pos = 0;
    m_state.maxrows = 0;
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

void QWoLineNoise::refreshMultiLine()
{
    char seq[64];
    int plen = m_prompt.length();
    int rows = (plen+m_state.buf.length()+column()-1)/column(); /* rows used by current buf. */
    int rpos = (plen+m_state.oldpos+column())/column(); /* cursor relative row. */
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
            && (m_state.pos + m_prompt.length()) % column() == 0) {
        ab.append('\n');
        n = snprintf(seq, 64, "\r");
        ab.append(seq, n);
        rows++;
        if (rows > m_state.maxrows) {
            m_state.maxrows = rows;
        }
    }
    /* Move cursor to right position. */
    int rpos2 = (m_prompt.length()+m_state.pos+column())/column(); /* current cursor relative row. */
    /* Go up till we reach the expected positon. */
    if (rows-rpos2 > 0) {
        n = snprintf(seq,64,"\x1b[%dA", rows-rpos2);
        ab.append(seq, n);
    }
    /* Set column. */
    int col = (m_prompt.length()+m_state.pos) % column();
    if (col){
        n = snprintf(seq,64,"\r\x1b[%dC", col);
    } else {
        n = snprintf(seq,64,"\r");
    }
    ab.append(seq, n);
    m_state.oldpos = m_state.pos;
    m_term->parseSequenceText(ab);
}
