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
    } LineNoiseState;

public:
    explicit QWoLineNoise(QTermWidget *term);
    void parse(const QByteArray& buf);
    void setPrompt(const QByteArray& prompt);

protected:
    virtual void handleCommand(const QByteArray& line) = 0;
    virtual void handleComplete() = 0;
    virtual QByteArray handleShowHints(QByteArray& line, int *pclr, int *pbold) = 0;
private:
    void reset();
    char completeLine();
    void editInsert(char c);
    void editMoveEnd();
    void refreshLine();
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

private:
    QByteArray m_prompt;
    QPointer<QTermWidget> m_term;
    LineNoiseState m_state;
};
