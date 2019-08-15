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
        QByteArray prompt; /* Prompt to display. */
        int pos;         /* Current cursor position. */
        int oldpos;      /* Previous refresh cursor position. */
        int cols;        /* Number of columns in terminal. */
        int maxrows;     /* Maximum num of rows used so far (multiline mode) */
    } LineNoiseState;

public:
    explicit QWoLineNoise(QTermWidget *term, const QByteArray& prompt, QObject *parent = nullptr);
    QByteArray append(const QByteArray& buf);
    QByteArray parse(const QByteArray& buf);
    void setColumn(int n);
signals:
    void result(const QByteArray& buf);
public slots:
private:
    QByteArray handleCommand(const QByteArray& cmd);
    QByteArray handleResult(const QByteArray& data);
    void reset();
    char completeLine();
    void editInsert(char c);
    void editMoveEnd();
    void refreshLine();
    void refreshMultiLine();
private:
    const QByteArray m_prompt;
    QPointer<QTermWidget> m_term;
    QByteArray m_line;

    LineNoiseState m_state;
};
