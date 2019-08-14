#pragma once

#include <QObject>
#include <QByteArray>

class QWoReadLine : public QObject
{
    Q_OBJECT
public:
    explicit QWoReadLine(const QByteArray& prompt, QObject *parent = nullptr);
    QByteArray append(const QByteArray& buf);

signals:
    void result(const QByteArray& buf);
public slots:
private:
    QByteArray handleCommand(const QByteArray& cmd);
    QByteArray handleResult(const QByteArray& data);
private:
    const QByteArray m_prompt;
    QByteArray m_line;
};
