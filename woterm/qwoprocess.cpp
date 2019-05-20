#include "qwoprocess.h"

#include <qtermwidget.h>

#include <QApplication>
#include <QDebug>
#include <QProcess>
#include <QMenu>
#include <QClipboard>


QWoProcess::QWoProcess(QObject *parent)
    : QObject (parent)
    , m_process(new QProcess())
{
    QObject::connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    QObject::connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
}

QWoProcess::~QWoProcess()
{
    delete m_process;
}

QString QWoProcess::program() const
{
    return m_process->program();
}

void QWoProcess::setProgram(const QString &program)
{
    m_process->setProgram(program);
}

QStringList QWoProcess::arguments() const
{
    return m_process->arguments();
}

void QWoProcess::setArguments(const QStringList &arguments)
{
    return m_process->setArguments(arguments);
}

QStringList QWoProcess::environment() const
{
    return m_process->environment();
}

void QWoProcess::setEnvironment(const QStringList &env)
{
    m_process->setEnvironment(env);
}

void QWoProcess::start()
{
    m_process->start();
}

QByteArray QWoProcess::readAllStandardOutput()
{
    return m_process->readAllStandardOutput();
}

QByteArray QWoProcess::readAllStandardError()
{
    return m_process->readAllStandardError();
}

void QWoProcess::write(const QByteArray &data)
{
    if(writeFilter(data)) {
        return;
    }
    m_process->setCurrentWriteChannel(QProcess::StandardOutput);
    m_process->write(data);
}

void QWoProcess::writeError(const QByteArray &data)
{
    if(writeFilter(data)) {
        return;
    }
    m_process->setCurrentWriteChannel(QProcess::StandardError);
    m_process->write(data);
}

#ifdef Q_OS_WIN
#include <Windows.h>
void QWoProcess::enableDebugConsole(bool on)
{
    m_process->setCreateProcessArgumentsModifier([on] (QProcess::CreateProcessArguments *cpa){
        cpa->flags = CREATE_NEW_CONSOLE;
        cpa->startupInfo->dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
        cpa->startupInfo->wShowWindow = on ? SW_SHOW : SW_HIDE;
    });
}
#else
void QWoProcess::enableDebugConsole(bool on)
{
}
#endif

bool QWoProcess::readStandardOutputFilter()
{
    return false;
}

bool QWoProcess::readStandardErrorFilter()
{
    return false;
}

bool QWoProcess::finishFilter(int code)
{
    return false;
}

bool QWoProcess::writeFilter(const QByteArray &data)
{
    return false;
}

void QWoProcess::setTermWidget(QTermWidget *widget)
{
    m_term = widget;
}

void QWoProcess::prepareContextMenu(QMenu *menu)
{

}

void QWoProcess::onReadyReadStandardOutput()
{
    if(readStandardOutputFilter()) {
        return;
    }
    emit readyReadStandardOutput();
}

void QWoProcess::onReadyReadStandardError()
{
    if(readStandardErrorFilter()) {
        return;
    }
    emit readyReadStandardError();
}

void QWoProcess::onFinished(int code)
{
    if(finishFilter(code)) {
        return ;
    }
    emit finished(code);
}
