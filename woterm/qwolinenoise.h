#pragma once

#include <QObject>
#include <QPointer>
#include <QByteArray>

class QTermWidget;

class QWoLineNoise : public QObject
{
    Q_OBJECT
private:
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
    explicit QWoLineNoise(QTermWidget *term, const QByteArray& prompt, QObject *parent = nullptr);
    void parse(const QByteArray& buf);
    void setColumn(int n);
signals:
    void command(const QByteArray& line);

private:
    void handleCommand(const QByteArray& buf);
    void reset();
    char completeLine();
    void editInsert(char c);
    void editMoveEnd();
    void refreshLine();
    void refreshMultiLine();
    int termColumn();
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
    const QByteArray m_prompt;
    QPointer<QTermWidget> m_term;
    QByteArray m_line;

    LineNoiseState m_state;
};
