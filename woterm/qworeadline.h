#ifndef QWOREADLINE_H
#define QWOREADLINE_H

#include <QObject>

class QWoReadLine : public QObject
{
    Q_OBJECT
public:
    explicit QWoReadLine(const QByteArray& prompt, QObject *parent = nullptr);
    void append(const QByteArray& buf);

signals:
    void result(const QByteArray& buf);
public slots:
private:
    void handleCommand(const QByteArray& cmd);
private:
    const QByteArray m_prompt;
    QByteArray m_line;
};

#endif // QWOREADLINE_H
