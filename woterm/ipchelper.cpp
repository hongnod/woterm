#include "ipchelper.h"

#include <QLocalSocket>
#include <QDataStream>

FunArgReader::FunArgReader(QLocalSocket *local, QObject *parent)
    :QObject (parent)
{
    m_socket = local;
}

void FunArgReader::readAll() {
    m_buf.append(m_socket->readAll());
}

QStringList FunArgReader::next() {
    int *plen = reinterpret_cast<int*>(m_buf.data());
    int length = plen[0] + int(sizeof(int));
    if(m_buf.size() < length) {
        return QStringList();
    }
    QByteArray funArgs = m_buf.mid(sizeof(int), plen[0]);
    m_buf.remove(0, length);
    QDataStream in(funArgs);
    QStringList data;
    in >> data;
    return data;
}

FunArgWriter::FunArgWriter(QLocalSocket *local, QObject *parent)
    :QObject (parent)
{
    m_socket = local;
}

void FunArgWriter::write(const QStringList &funArgs) {
    QByteArray buf;
    QDataStream in(&buf, QIODevice::WriteOnly);
    in << funArgs;
    int length = buf.length();
    m_socket->write((char*)&length, sizeof(int));
    m_socket->write((char*)buf.data(), length);
}
