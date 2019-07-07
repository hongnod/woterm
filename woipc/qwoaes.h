#pragma once

#include "woipc_global.h"

#include <QObject>
#include <QPointer>
#include <QMap>

class QAesWrap;

class QWoAes : public QObject
{
    Q_OBJECT
public:
    explicit QWoAes(QObject *parent=nullptr);
    virtual ~QWoAes();

    int create(const QByteArray & passwprd);
    bool encrypt(int hdl, const QByteArray& in, QByteArray& out);
    bool decrypt(int hdl, const QByteArray& in, QByteArray& out);
    void destroy(int hdl);
private:
    QMap<int, QPointer<QAesWrap>> m_aes;
};

#ifdef __cplusplus
extern "C" {
#endif

int WOIPCSHARED_EXPORT AesNew(const char *password);
bool WOIPCSHARED_EXPORT AesEncode(int hdl, const char *pin, int len, const char *pout, int *plen);
bool WOIPCSHARED_EXPORT AesDecode(int hdl, const char *pin, int len, const char* pout, int *plen);
void WOIPCSHARED_EXPORT AesDelete(int hdl);

#ifdef __cplusplus
}
#endif
