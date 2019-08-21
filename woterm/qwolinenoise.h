#pragma once

#include <QObject>
#include <QPointer>
#include <QByteArray>

class QTermWidget;

class QWoLineNoise
{
protected:
    /* The linenoiseState structure represents the state during line editing.
     * We pass this state to functions implementing specific editing
     * functionalities. */
    typedef struct {
        QByteArray buf;          /* Edited line buffer. */
        int pos;         /* Current cursor position. */
        int oldpos;      /* Previous refresh cursor position. */
        int maxrows;     /* Maximum num of rows used so far (multiline mode) */
        QList<QByteArray> completes;
        int completeIndex;
    } LineNoiseState;

public:
    explicit QWoLineNoise(QTermWidget *term);
    void parse(const QByteArray& buf);
    void setPrompt(const QByteArray& prompt);

protected:
    virtual void handleCommand(const QByteArray& line) = 0;
    virtual QList<QByteArray> handleComplete(const QByteArray& line) = 0;
    virtual QByteArray handleShowHints(QByteArray& line, int *pclr, int *pbold) = 0;
protected:
    void refreshLine();
private:
    void reset();
    void normalParse(const QByteArray& buf);
    void completeParse(const QByteArray& buf);

    void editInsert(char c);
    void editMoveEnd();
    void refreshMultiLine();
    QByteArray refreshShowHints();
    int column();
    void editDelete();
    void editHistoryPrev();
    void editHistoryNext();
    void editMoveRight();
    void editMoveLeft();
    void editMoveHome();
    void editBackspace();
    void clearScreen();
    void editDeletePrevWord();
    void beep();
private:
    QByteArray m_prompt;
    QPointer<QTermWidget> m_term;
    LineNoiseState m_state;
};
