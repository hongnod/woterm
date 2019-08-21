#include "qwoshellwidget.h"
#include "qwoprocess.h"
#include "qwosetting.h"
#include "qwosshconf.h"
#include "qwosshprocess.h"
#include "qwoutils.h"
#include "qwoglobal.h"
#include "qwoshellwidgetimpl.h"
#include "qwolinenoise.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QClipboard>
#include <QSplitter>


QWoShellWidget::QWoShellWidget(QWidget *parent)
    : QTermWidget (parent)
    , QWoLineNoise (this)
    , m_bScrollToEnd(false)
{
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

    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    QObject::connect(this, SIGNAL(sendData(const QByteArray&)), this, SLOT(onSendData(const QByteArray&)));


    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    loadCommandList();
}

QWoShellWidget::~QWoShellWidget()
{
    if(m_menu) {
        delete m_menu;
    }
}

void QWoShellWidget::closeAndDelete()
{
    m_bexit = true;
    deleteLater();
    QSplitter *splitParent = qobject_cast<QSplitter*>(parent());
    if(splitParent == nullptr) {
        return;
    }
    int cnt = splitParent->count();
    if(cnt == 1) {
        splitParent->deleteLater();
    }
}

void QWoShellWidget::onTimeout()
{
    if(m_bScrollToEnd) {
        scrollToEnd();
    }
}

void QWoShellWidget::onSendData(const QByteArray &buf)
{
    parse(buf);
    m_bScrollToEnd = true;
}

void QWoShellWidget::onCopyToClipboard()
{
    QClipboard *clip = QGuiApplication::clipboard();
    QString selTxt = selectedText();
    if(!selTxt.isEmpty()) {
        clip->setText(selTxt);
    }
}

void QWoShellWidget::onPasteFromClipboard()
{
    QClipboard *clip = QGuiApplication::clipboard();
    QString clipTxt = clip->text();
    QByteArray buf = clipTxt.toUtf8();
}

void QWoShellWidget::onVerticalSplitView()
{
    splitWidget(true);
}

void QWoShellWidget::onHorizontalSplitView()
{
    splitWidget(false);
}

void QWoShellWidget::onCloseThisSession()
{
    closeAndDelete();
}

void QWoShellWidget::contextMenuEvent(QContextMenuEvent *e)
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
        QAction *close = m_menu->addAction(tr("Close Session"));
        QObject::connect(close, SIGNAL(triggered()), this, SLOT(onCloseThisSession()));
    }

    QString selTxt = selectedText();
    m_copy->setDisabled(selTxt.isEmpty());
    QClipboard *clip = QGuiApplication::clipboard();
    QString clipTxt = clip->text();
    m_paste->setDisabled(clipTxt.isEmpty());

    m_menu->exec(QCursor::pos());
}

void QWoShellWidget::closeEvent(QCloseEvent *event)
{
    emit aboutToClose(event);
    if(event->isAccepted()) {
        return;
    }
    QTermWidget::closeEvent(event);
}

void QWoShellWidget::resizeEvent(QResizeEvent *event)
{
    QTermWidget::resizeEvent(event);
}

void QWoShellWidget::initDefault()
{
    QString val = QWoSetting::value("property/default").toString();
    QVariantMap mdata = QWoUtils::qBase64ToVariant(val).toMap();
    resetProperty(mdata);
}

void QWoShellWidget::resetProperty(QVariantMap mdata)
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

void QWoShellWidget::splitWidget(bool vertical)
{
    QSplitter *splitParent = qobject_cast<QSplitter*>(parent());
    if(splitParent == nullptr) {
        return;
    }
    int cnt = splitParent->count();
    QSplitter *splitter = splitParent;
    if(cnt > 1) {
        int idx = splitParent->indexOf(this);
        QSplitter *splitNew = new QSplitter(this);
        splitParent->replaceWidget(idx, splitNew);
        splitNew->addWidget(this);
        splitter = splitNew;
        splitter->setHandleWidth(1);
        splitter->setOpaqueResize(false);
        splitter->setAutoFillBackground(true);
        QPalette pal(Qt::gray);
        splitter->setPalette(pal);
    }

    splitter->setOrientation(vertical ? Qt::Vertical : Qt::Horizontal);
    QWoShellWidget *term = new QWoShellWidget(splitter);
    splitter->addWidget(term);

    QList<int> ls;
    ls << 1 << 1;
    splitter->setSizes(ls);
}

void QWoShellWidget::loadCommandList()
{
    m_cmds << "dir" << "ls" << "ls -l" << "ls -lh" << "rm" << "grep" << "pwd";
}

void QWoShellWidget::handleCommand(const QByteArray& line)
{

}

QList<QByteArray> QWoShellWidget::handleComplete(const QByteArray &line)
{
    QList<QByteArray> lsTmp;
    for(int i = 0; i < m_cmds.count(); i++) {
        QByteArray cmd = m_cmds.at(i);
        if(cmd.startsWith(line)) {
            lsTmp.append(cmd);
        }
    }
    return lsTmp;
}

QByteArray QWoShellWidget::handleShowHints(QByteArray &line, int *pclr, int *pbold)
{
    if(line.isEmpty()) {
        return QByteArray();
    }
    for(int i = 0; i < m_cmds.count(); i++) {
        QByteArray cmd = m_cmds.at(i);
        if(cmd.startsWith(line)) {
            *pbold = 1;
            return cmd.mid(line.length());
        }
    }
    return QByteArray();
}
