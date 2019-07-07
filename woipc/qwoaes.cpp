#include "qwoaes.h"
#include "qaeswrap.h"

static QWoAes woAes;
//https://github.com/dushibaiyu/QAes
#define FIX_SALTS_STRING  ("woterm.wingo.he")
QWoAes::QWoAes(QObject *parent)
    :QObject (parent)
{

}

QWoAes::~QWoAes()
{

}

int QWoAes::create(const QByteArray &passwprd)
{
    static QAtomicInt aint = 5100;
    int icnt = aint++;
    QAesWrap *aes = new QAesWrap(passwprd, FIX_SALTS_STRING, QAesWrap::AES_256);
    m_aes.insert(icnt, aes);
    return icnt;
}

bool QWoAes::encrypt(int hdl, const QByteArray &in, QByteArray &out)
{
    QAesWrap *aes = m_aes.value(hdl);
    if(aes == nullptr) {
        return false;
    }
    return aes->encrypt(in, out, QAesWrap::AES_CTR, QAesWrap::None);
}

bool QWoAes::decrypt(int hdl, const QByteArray &in, QByteArray &out)
{
    QAesWrap *aes = m_aes.value(hdl);
    if(aes == nullptr) {
        return false;
    }
    return aes->decrypt(in, out, QAesWrap::AES_CTR, QAesWrap::None);
}

void QWoAes::destroy(int hdl)
{
    QAesWrap *aes = m_aes.take(hdl);
    if(aes == nullptr) {
        return;
    }
    delete aes;
}



int AesNew(const char *password)
{
    return woAes.create(password);
}

bool AesEncode(int hdl, const char *pin, int len, const char *pout, int *plen)
{
    QByteArray in(pin, len);
    QByteArray out(pout, *plen);
    if(len < *plen) {
        return false;
    }
    return woAes.encrypt(hdl, in, out);
}

bool AesDecode(int hdl, const char *pin, int len, const char *pout, int *plen)
{
    QByteArray in(pin, len);
    QByteArray out(pout, *plen);
    if(len > *plen) {
        return false;
    }
    return woAes.decrypt(hdl, in, out);
}

void AesDelete(int hdl)
{
    return woAes.destroy(hdl);
}
