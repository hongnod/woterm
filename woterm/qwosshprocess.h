#pragma once

#include "qwoprocess.h"

#include <QPointer>

class QTermWidget;
class QLocalServer;

class QWoSshProcess : public QWoProcess
{
    Q_OBJECT
public:
    explicit QWoSshProcess();
private:
    QPointer<QLocalServer> m_serer;
};
