#pragma once

#include "qwoprocess.h"

#include <QPointer>

class QTermWidget;

class QWoSshProcess : public QWoProcess
{
    Q_OBJECT
public:
    explicit QWoSshProcess();
};
