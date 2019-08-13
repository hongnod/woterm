#ifndef QWOREADLINE_H
#define QWOREADLINE_H

#include <QObject>

class qworeadline : public QObject
{
    Q_OBJECT
public:
    explicit qworeadline(const QByteArray& prompt, QObject *parent = nullptr);
    void append(const QByteArray& buf);
signals:

public slots:
private:
    const QByteArray m_prompt;
    QByteArray m_line;
};

#endif // QWOREADLINE_H
