#include "qwotermwidget.h"
#include "qwoprocess.h"
#include "qwosetting.h"
#include "qwosshconf.h"
#include "qwosshprocess.h"
#include "qwoutils.h"
#include "qwoglobal.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QSplitter>


QWoTermWidget::QWoTermWidget(QWoProcess *process, QWidget *parent)
    : QTermWidget (parent)
    , m_process(process)
{
    m_process->setTermWidget(this);
    setAttribute(Qt::WA_DeleteOnClose);

    QFont font = QApplication::font();
    font.setFamily(DEFAULT_FONT_FAMILY);
    font.setPointSize(DEFAULT_FONT_SIZE);
    setTerminalFont(font);
    setScrollBarPosition(QTermWidget::ScrollBarRight);
    setColorScheme(DEFAULT_COLOR_SCHEMA);
    setKeyBindings(DEFAULT_KEYBOARD_BINDING);

    setBlinkingCursor(true);
    setBidiEnabled(true);

    initDefault();
    initCustom();


    QObject::connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    QObject::connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));
    QObject::connect(m_process, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
    QObject::connect(this, SIGNAL(sendData(const QByteArray&)), this, SLOT(onSendData(const QByteArray&)));
}

QWoTermWidget::~QWoTermWidget()
{
    if(m_menu) {
        delete m_menu;
    }
}

QWoProcess *QWoTermWidget::process()
{
    return m_process;
}

void QWoTermWidget::onTimeout()
{
    qDebug() << "onTimeout()";
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
}

void QWoTermWidget::onSendData(const QByteArray &buf)
{
    scrollToEnd();
    m_process->write(buf);
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
    m_process->write(buf);
}

void QWoTermWidget::onVerticalSplitView()
{
    int sz = height() / 2;
    splitWidget(sz, true);
}

void QWoTermWidget::onHorizontalSplitView()
{
    int sz = width() / 2;
    splitWidget(sz, false);
}

void QWoTermWidget::contextMenuEvent(QContextMenuEvent *e)
{
    if(m_menu == nullptr) {
        m_menu = new QMenu();
        m_copy = m_menu->addAction(tr("Copy"));
        QObject::connect(m_copy, SIGNAL(triggered()), this, SLOT(onCopyToClipboard()));
        m_paste = m_menu->addAction(tr("Paste"));
        QObject::connect(m_paste, SIGNAL(triggered()), this, SLOT(onPasteFromClipboard()));        
        QAction *vsplit = m_menu->addAction(tr("Split Vertical"));
        QObject::connect(vsplit, SIGNAL(triggered()), this, SLOT(onVerticalSplitView()));
        QAction *hsplit = m_menu->addAction(tr("Split Horizontal"));
        QObject::connect(hsplit, SIGNAL(triggered()), this, SLOT(onHorizontalSplitView()));
    }

    QString selTxt = selectedText();
    m_copy->setDisabled(selTxt.isEmpty());
    QClipboard *clip = QGuiApplication::clipboard();
    QString clipTxt = clip->text();
    m_paste->setDisabled(clipTxt.isEmpty());

    m_process->prepareContextMenu(m_menu);

    m_menu->exec(QCursor::pos());
}

void QWoTermWidget::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QTermWidget::closeEvent(event);
}

void QWoTermWidget::initDefault()
{
    QString val = QWoSetting::value("property/default").toString();
    QVariantMap mdata = QWoUtils::qBase64ToVariant(val).toMap();
    resetProperty(mdata);
}

void QWoTermWidget::initCustom()
{
    QWoSshProcess *sshprocess = qobject_cast<QWoSshProcess*>(m_process);
    if(sshprocess == nullptr) {
        return;
    }
    QString target = sshprocess->target();
    HostInfo hi = QWoSshConf::instance()->findHostInfo(target);
    QVariantMap mdata = QWoUtils::qBase64ToVariant(hi.property).toMap();
    resetProperty(mdata);
}

void QWoTermWidget::resetProperty(QVariantMap mdata)
{
    if(mdata.isEmpty()) {
        return;
    }
    QString schema = mdata.value("colorSchema", DEFAULT_COLOR_SCHEMA).toString();
    setColorScheme(schema);
    QString binding = mdata.value("keyBinding", DEFAULT_KEYBOARD_BINDING).toString();
    setKeyBindings(binding);
    QString fontName = mdata.value("fontName", DEFAULT_FONT_FAMILY).toString();
    int fontSize = mdata.value("fontSize", DEFAULT_FONT_SIZE).toInt();
    QFont ft(fontName, fontSize);
    setTerminalFont(ft);
    QString cursorType = mdata.value("cursorType", "block").toString();
    if(cursorType.isEmpty() || cursorType == "block") {
        setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::BlockCursor);
    }else if(cursorType == "underline") {
        setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::UnderlineCursor);
    }else {
        setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::IBeamCursor);
    }
    int lines = mdata.value("historyLength", DEFAULT_HISTORY_LINE_LENGTH).toInt();
    setHistorySize(lines);
}

void QWoTermWidget::splitWidget(int sz, bool vertical)
{
    QSplitter *splitParent = qobject_cast<QSplitter*>(parent());
    if(splitParent == nullptr) {
        return;
    }
    QWoSshProcess *sshproc = qobject_cast<QWoSshProcess*>(m_process);
    if(sshproc == nullptr) {
        return;
    }
    int idx = splitParent->indexOf(this);
    QSplitter *splitNew = new QSplitter(this);
    splitParent->replaceWidget(idx, splitNew);
    splitNew->addWidget(this);

    splitNew->setOrientation(vertical ? Qt::Vertical : Qt::Horizontal);
    QString target = sshproc->target();
    QWoSshProcess *process = new QWoSshProcess(target, this);
    QWoTermWidget *term = new QWoTermWidget(process, splitNew);
    splitNew->addWidget(term);
    process->start();

    QList<int> ls;
    ls << sz << sz;
    splitNew->setSizes(ls);
}
