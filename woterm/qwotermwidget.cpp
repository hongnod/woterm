#include "qwotermwidget.h"
#include "qwoprocess.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>

#ifdef Q_OS_MACOS
#define DEFAULT_FONT_FAMILY                   "Menlo"
#elif defined(Q_OS_WIN)
#define DEFAULT_FONT_FAMILY                   "Courier New"
#else
#define DEFAULT_FONT_FAMILY                   "Monospace"
#endif

QWoTermWidget::QWoTermWidget(QWoProcess *process, QWidget *parent)
    : QTermWidget (parent)
    , m_process(process)
{
    m_process->setTermWidget(this);

    QFont font = QApplication::font();
    font.setFamily(DEFAULT_FONT_FAMILY);
    font.setPointSize(12);
    setTerminalFont(font);
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setColorScheme("Linux");
    setKeyBindings("linux");

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

    QObject::connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    QObject::connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    QObject::connect(this, SIGNAL(sendData(const QByteArray&)), this, SLOT(onSendData(const QByteArray&)));
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
    QByteArray out = m_process->readAllStandardOutput();
    parseSequenceText(out);
}

void QWoTermWidget::onReadyReadStandardError()
{
    QByteArray err = m_process->readAllStandardError();
    parseSequenceText(err);
}

void QWoTermWidget::onFinished(int code)
{
    qDebug() << "exitcode" << code;
    QApplication::exit(code);
}

void QWoTermWidget::onSendData(const QByteArray &buf)
{
    m_process->write(buf.data(), buf.length());
}

void QWoTermWidget::onCopyToClipboard()
{
    QClipboard *clip = QGuiApplication::clipboard();
    QString selTxt = selectedText();
    if(!selTxt.isEmpty()) {
        clip->setText(selTxt);
    }
}

void QWoTermWidget::onPasteFromClipboard()
{
    QClipboard *clip = QGuiApplication::clipboard();
    QString clipTxt = clip->text();
    QByteArray buf = clipTxt.toUtf8();
    m_process->write(buf.data(), buf.length());
}

void QWoTermWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if(m_menu == nullptr) {
        m_menu = new QMenu(this);
        m_copy = m_menu->addAction(tr("Copy"));
        QObject::connect(m_copy, SIGNAL(triggered()), this, SLOT(onCopyToClipboard()));
        m_paste = m_menu->addAction(tr("Paste"));
        QObject::connect(m_paste, SIGNAL(triggered()), this, SLOT(onPasteFromClipboard()));
    }

    QString selTxt = selectedText();
    m_copy->setDisabled(selTxt.isEmpty());
    QClipboard *clip = QGuiApplication::clipboard();
    QString clipTxt = clip->text();
    m_paste->setDisabled(clipTxt.isEmpty());

    m_process->prepareContextMenu(m_menu);

    m_menu->exec(QCursor::pos());
}
