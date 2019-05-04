#include "qwotermwidget.h"

#include <QApplication>
#include <QDebug>
#include <QProcess>

#ifdef Q_OS_MACOS
#define DEFAULT_FONT_FAMILY                   "Menlo"
#elif defined(Q_OS_WIN)
#define DEFAULT_FONT_FAMILY                   "Courier New"
#else
#define DEFAULT_FONT_FAMILY                   "Monospace"
#endif

QWoTermWidget::QWoTermWidget(QWidget *parent)
    :QTermWidget (parent)
{
    QFont font = QApplication::font();
    font.setFamily(DEFAULT_FONT_FAMILY);
    font.setPointSize(12);
    setTerminalFont(font);
    setScrollBarPosition(QTermWidget::ScrollBarRight);

    const auto arguments = QApplication::arguments();
    for (const QString& arg : arguments)
    {
        if (availableColorSchemes().contains(arg)){
            setColorScheme(arg);
        }
        if (availableKeyBindings().contains(arg)){
            setKeyBindings(arg);
        }
    }

#if 0
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(5000);
#endif

    m_pProcess = new QProcess(this);
    QObject::connect(m_pProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    QObject::connect(m_pProcess, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
    QObject::connect(m_pProcess, SIGNAL(finished(int)), this, SLOT(onFinish(int)));
    QObject::connect(this, SIGNAL(sendData(const QByteArray&)), this, SLOT(onSendData(const QByteArray&)));

    m_pProcess->setProgram("D:\\woterm\\openssh\\win32\\sbin\\x64\\Debug\\sshproxy.exe");
    QStringList args;
    args << "-F" << "D:\\config" << "jump";
    m_pProcess->setArguments(args);
    m_pProcess->start();
}

void QWoTermWidget::onTimeout()
{
    qDebug() << "onTimeout()";

    parseSequenceText("\x1B[?3l");
    parseSequenceText("\x1B[?3h");
    parseSequenceText("jEh你好abc");
    parseSequenceText("\033[31mHello\033[0m World");
    parseSequenceText("error");
    char buf[] = { 0x0d, 0x0a, 0x1b, 0x5b, 0x30, 0x6d, 0x1b, 0x5b, 0x30, 0x31, 0x3b, 0x33, 0x34, 0x6d, 0x63, 0x6f, 0x6e, 0x66, 0x1b, 0x5b, 0x30, 0x6d, 0x20,
                   0x20, 0x1b, 0x5b, 0x30, 0x31, 0x3b, 0x33, 0x34, 0x6d, 0x76, 0x6f, 0x6c, 0x75, 0x6d, 0x65, 0x1b, 0x5b, 0x30, 0x6d, 0x0d, 0x0a, 0x1b, 0x5d,
                   0x30, 0x3b, 0x61, 0x62, 0x63, 0x40, 0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x3a, 0x7e, 0x07, 0x5b, 0x61, 0x62, 0x63, 0x40,
                   0x6c, 0x6f, 0x63, 0x61, 0x6c, 0x68, 0x6f, 0x73, 0x74, 0x20, 0x7e, 0x5d, 0x24, 0x20, 0x00};
    parseSequenceText(buf);
}

void QWoTermWidget::onReadyReadStandardOutput()
{
    QByteArray out = m_pProcess->readAllStandardOutput();
    parseSequenceText(out);
}

void QWoTermWidget::onReadyReadStandardError()
{
    QByteArray err = m_pProcess->readAllStandardError();
    parseSequenceText(err);
}

void QWoTermWidget::onFinish(int code)
{
    qDebug() << "exitcode" << code;
}

void QWoTermWidget::onSendData(const QByteArray &buf)
{
    m_pProcess->write(buf.data(), buf.length());
}
