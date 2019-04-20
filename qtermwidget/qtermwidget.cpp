/*  Copyright (C) 2008 e_k (e_k@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include <QLayout>
#include <QBoxLayout>
#include <QtDebug>
#include <QDir>
#include <QMessageBox>
#include <QMetaObject>
#include <QObject>

#include "ColorTables.h"
#include "Session.h"
#include "Screen.h"
#include "ScreenWindow.h"
#include "Emulation.h"
#include "TerminalDisplay.h"
#include "KeyboardTranslator.h"
#include "ColorScheme.h"
#include "SearchBar.h"
#include "qtermwidget.h"


#ifdef Q_OS_MACOS
// Qt does not support fontconfig on macOS, so we need to use a "real" font name.
#define DEFAULT_FONT_FAMILY                   "Menlo"
#else
#define DEFAULT_FONT_FAMILY                   "Monospace"
#endif

#define STEP_ZOOM 1

using namespace Konsole;

void *createTermWidget(int startnow, void *parent)
{
    return (void*) new QTermWidget(startnow, (QWidget*)parent);
}

struct TermWidgetImpl {
    TermWidgetImpl(QWidget* parent = 0);

    TerminalDisplay *m_terminalDisplay;
    Session *m_session;

    Session* createSession(QWidget* parent);
    TerminalDisplay* createTerminalDisplay(Session *session, QWidget* parent);
};

TermWidgetImpl::TermWidgetImpl(QWidget* parent)
{
    this->m_session = createSession(parent);
    this->m_terminalDisplay = createTerminalDisplay(this->m_session, parent);
}


Session *TermWidgetImpl::createSession(QWidget* parent)
{
    Session *session = new Session(parent);

    session->setTitle(Session::NameRole, QLatin1String("QTermWidget"));

    /* Thats a freaking bad idea!!!!
     * /bin/bash is not there on every system
     * better set it to the current $SHELL
     * Maybe you can also make a list available and then let the widget-owner decide what to use.
     * By setting it to $SHELL right away we actually make the first filecheck obsolete.
     * But as iam not sure if you want to do anything else ill just let both checks in and set this to $SHELL anyway.
     */
    //session->setProgram("/bin/bash");

    session->setProgram(QString::fromLocal8Bit(qgetenv("SHELL")));



    QStringList args = QStringList(QString());
    session->setArguments(args);
    session->setAutoClose(true);

    session->setCodec(QTextCodec::codecForName("UTF-8"));

    session->setFlowControlEnabled(true);
    session->setHistoryType(HistoryTypeBuffer(1000));

    session->setDarkBackground(true);

    session->setKeyBindings(QString());
    return session;
}

TerminalDisplay *TermWidgetImpl::createTerminalDisplay(Session *session, QWidget* parent)
{
//    TerminalDisplay* display = new TerminalDisplay(this);
    TerminalDisplay* display = new TerminalDisplay(parent);

    display->setBellMode(TerminalDisplay::NotifyBell);
    display->setTerminalSizeHint(true);
    display->setTripleClickMode(TerminalDisplay::SelectWholeLine);
    display->setTerminalSizeStartup(true);

    display->setRandomSeed(session->sessionId() * 31);

    return display;
}


QTermWidget::QTermWidget(int startnow, QWidget *parent)
    : QWidget(parent)
{
    init(startnow);   
}

QTermWidget::QTermWidget(QWidget *parent)
    : QWidget(parent)
{
    init(1);
}

void QTermWidget::selectionChanged(bool textSelected)
{
    emit copyAvailable(textSelected);
}

void QTermWidget::onKeyPressedSignal(QKeyEvent *e)
{
    QByteArray data;
    switch (e->key()) {
    case Qt::Key_Backspace:
        data.push_back(char(8));
        break;
    case Qt::Key_F1:
        //parse("\x1B[2J\x1B[3g\x1B[1;1H\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[3C\x1BH\x1B[1;4H\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[0g\x1B[6C\x1B[1;7H\x1B[1g\x1B[2g\x1B[1;1H\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\t*\x1B[2;2H     *     *     *     *     *     *     *     *     *     *     *     *     *\x1B[4;1HTest of TAB setting/resetting. These two lines\r\nshould look the same. Push <RETURN>");
        parse("\x1B[?5h\x1B[?3h\x1B[2J\x1B[1;1H\x1B[3g\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[8C\x1BH\x1B[1;1H12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901\x1B[3;3HThis is 132 column mode, light background.\x1B[4;4HThis is 132 column mode, light background.\x1B[5;5HThis is 132 column mode, light background.\x1B[6;6HThis is 132 column mode, light background.\x1B[7;7HThis is 132 column mode, light background.\x1B[8;8HThis is 132 column mode, light background.\x1B[9;9HThis is 132 column mode, light background.\x1B[10;10HThis is 132 column mode, light background.\x1B[11;11HThis is 132 column mode, light background.\x1B[12;12HThis is 132 column mode, light background.\x1B[13;13HThis is 132 column mode, light background.\x1B[14;14HThis is 132 column mode, light background.\x1B[15;15HThis is 132 column mode, light background.\x1B[16;16HThis is 132 column mode, light background.\x1B[17;17HThis is 132 column mode, light background.\x1B[18;18HThis is 132 column mode, light background.\x1B[19;19HThis is 132 column mode, light background.\x1B[20;20HThis is 132 column mode, light background.Push <RETURN>");
        break;
    }
}

void QTermWidget::find()
{
    search(true, false);
}

void QTermWidget::findNext()
{
    search(true, true);
}

void QTermWidget::findPrevious()
{
    search(false, false);
}

void QTermWidget::search(bool forwards, bool next)
{
    int startColumn, startLine;

    if (next) // search from just after current selection
    {
        m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionEnd(startColumn, startLine);
        startColumn++;
    }
    else // search from start of current selection
    {
        m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionStart(startColumn, startLine);
    }

    qDebug() << "current selection starts at: " << startColumn << startLine;
    qDebug() << "current cursor position: " << m_impl->m_terminalDisplay->screenWindow()->cursorPosition();

    QRegExp regExp(m_searchBar->searchText());
    regExp.setPatternSyntax(m_searchBar->useRegularExpression() ? QRegExp::RegExp : QRegExp::FixedString);
    regExp.setCaseSensitivity(m_searchBar->matchCase() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    HistorySearch *historySearch =
            new HistorySearch(m_impl->m_session->emulation(), regExp, forwards, startColumn, startLine, this);
    connect(historySearch, SIGNAL(matchFound(int, int, int, int)), this, SLOT(matchFound(int, int, int, int)));
    connect(historySearch, SIGNAL(noMatchFound()), this, SLOT(noMatchFound()));
    connect(historySearch, SIGNAL(noMatchFound()), m_searchBar, SLOT(noMatchFound()));
    historySearch->search();
}


void QTermWidget::matchFound(int startColumn, int startLine, int endColumn, int endLine)
{
    ScreenWindow* sw = m_impl->m_terminalDisplay->screenWindow();
    qDebug() << "Scroll to" << startLine;
    sw->scrollTo(startLine);
    sw->setTrackOutput(false);
    sw->notifyOutputChanged();
    sw->setSelectionStart(startColumn, startLine - sw->currentLine(), false);
    sw->setSelectionEnd(endColumn, endLine - sw->currentLine());
}

void QTermWidget::noMatchFound()
{
        m_impl->m_terminalDisplay->screenWindow()->clearSelection();
}

int QTermWidget::getShellPID()
{
    return m_impl->m_session->processId();
}

void QTermWidget::changeDir(const QString & dir)
{
    /*
       this is a very hackish way of trying to determine if the shell is in
       the foreground before attempting to change the directory.  It may not
       be portable to anything other than Linux.
    */
    QString strCmd;
    strCmd.setNum(getShellPID());
    strCmd.prepend(QLatin1String("ps -j "));
    strCmd.append(QLatin1String(" | tail -1 | awk '{ print $5 }' | grep -q \\+"));
    int retval = system(strCmd.toStdString().c_str());

    if (!retval) {
        QString cmd = QLatin1String("cd ") + dir + QLatin1Char('\n');
        sendText(cmd);
    }
}

QSize QTermWidget::sizeHint() const
{
    QSize size = m_impl->m_terminalDisplay->sizeHint();
    size.rheight() = 150;
    return size;
}

void QTermWidget::setTerminalSizeHint(bool on)
{
    m_impl->m_terminalDisplay->setTerminalSizeHint(on);
}

bool QTermWidget::terminalSizeHint()
{
    return m_impl->m_terminalDisplay->terminalSizeHint();
}

void QTermWidget::startShellProgram()
{
    if ( m_impl->m_session->isRunning() ) {
        return;
    }

    m_impl->m_session->run();
}

void QTermWidget::startTerminalTeletype()
{
    if ( m_impl->m_session->isRunning() ) {
        return;
    }

    m_impl->m_session->runEmptyPTY();
    // redirect data from TTY to external recipient
    connect( m_impl->m_session->emulation(), SIGNAL(sendData(const char *,int)),
             this, SIGNAL(sendData(const char *,int)) );
}

#define TRANSLATIONS_DIR "./translations"

void QTermWidget::init(int startnow)
{
    m_layout = new QVBoxLayout();
    m_layout->setMargin(0);
    setLayout(m_layout);

    // translations
    // First check $XDG_DATA_DIRS. This follows the implementation in libqtxdg
    QString d = QFile::decodeName(qgetenv("XDG_DATA_DIRS"));
    QStringList dirs = d.split(QLatin1Char(':'), QString::SkipEmptyParts);
    if (dirs.isEmpty()) {
        dirs.append(QString::fromLatin1("/usr/local/share"));
        dirs.append(QString::fromLatin1("/usr/share"));
    }
    dirs.append(QFile::decodeName(TRANSLATIONS_DIR));

    m_translator = new QTranslator(this);

    for (const QString& dir : dirs) {
        qDebug() << "Trying to load translation file from dir" << dir;
        if (m_translator->load(QLocale::system(), QLatin1String("qtermwidget"), QLatin1String(QLatin1String("_")), dir)) {
            qApp->installTranslator(m_translator);
            qDebug() << "Translations found in" << dir;
            break;
        }
    }

    m_impl = new TermWidgetImpl(this);
    m_layout->addWidget(m_impl->m_terminalDisplay);

    connect(m_impl->m_session, SIGNAL(bellRequest(QString)), m_impl->m_terminalDisplay, SLOT(bell(QString)));
    connect(m_impl->m_terminalDisplay, SIGNAL(notifyBell(QString)), this, SIGNAL(bell(QString)));

    connect(m_impl->m_session, SIGNAL(activity()), this, SIGNAL(activity()));
    connect(m_impl->m_session, SIGNAL(silence()), this, SIGNAL(silence()));
    connect(m_impl->m_session, &Session::profileChangeCommandReceived, this, &QTermWidget::profileChanged);
    connect(m_impl->m_session, &Session::receivedData, this, &QTermWidget::receivedData);

    // That's OK, FilterChain's dtor takes care of UrlFilter.
    UrlFilter *urlFilter = new UrlFilter();
    connect(urlFilter, &UrlFilter::activated, this, &QTermWidget::urlActivated);
    m_impl->m_terminalDisplay->filterChain()->addFilter(urlFilter);

    m_searchBar = new SearchBar(this);
    m_searchBar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);
    connect(m_searchBar, SIGNAL(searchCriteriaChanged()), this, SLOT(find()));
    connect(m_searchBar, SIGNAL(findNext()), this, SLOT(findNext()));
    connect(m_searchBar, SIGNAL(findPrevious()), this, SLOT(findPrevious()));
    m_layout->addWidget(m_searchBar);
    m_searchBar->hide();

    if (startnow && m_impl->m_session) {
        m_impl->m_session->run();
    }

    this->setFocus( Qt::OtherFocusReason );
    this->setFocusPolicy( Qt::WheelFocus );
    m_impl->m_terminalDisplay->resize(this->size());

    this->setFocusProxy(m_impl->m_terminalDisplay);
    connect(m_impl->m_terminalDisplay, SIGNAL(copyAvailable(bool)),
            this, SLOT(selectionChanged(bool)));
    connect(m_impl->m_terminalDisplay, SIGNAL(termGetFocus()),
            this, SIGNAL(termGetFocus()));
    connect(m_impl->m_terminalDisplay, SIGNAL(termLostFocus()),
            this, SIGNAL(termLostFocus()));
    connect(m_impl->m_terminalDisplay, SIGNAL(keyPressedSignal(QKeyEvent *)),
            this, SIGNAL(termKeyPressed(QKeyEvent *)));
//    m_impl->m_terminalDisplay->setSize(80, 40);

    QFont font = QApplication::font();
    font.setFamily(QLatin1String(DEFAULT_FONT_FAMILY));
    font.setPointSize(10);
    font.setStyleHint(QFont::TypeWriter);
    setTerminalFont(font);
    m_searchBar->setFont(font);

    setScrollBarPosition(NoScrollBar);
    setKeyboardCursorShape(Emulation::KeyboardCursorShape::BlockCursor);

    m_impl->m_session->addView(m_impl->m_terminalDisplay);

    connect(m_impl->m_session, SIGNAL(resizeRequest(QSize)), this, SLOT(setSize(QSize)));
    connect(m_impl->m_session, SIGNAL(finished()), this, SLOT(sessionFinished()));
    connect(m_impl->m_session, &Session::titleChanged, this, &QTermWidget::titleChanged);
    connect(m_impl->m_session, &Session::cursorChanged, this, &QTermWidget::cursorChanged);

    setFocusPolicy(Qt::StrongFocus);
    bool ok = QObject::connect(m_impl->m_terminalDisplay, SIGNAL(keyPressedSignal(QKeyEvent*)), this, SLOT(onKeyPressedSignal(QKeyEvent*)));
    qDebug() << "connect" << ok;
}


QTermWidget::~QTermWidget()
{
    delete m_impl;
    emit destroyed();
}


void QTermWidget::setTerminalFont(const QFont &font)
{
    m_impl->m_terminalDisplay->setVTFont(font);
}

QFont QTermWidget::getTerminalFont()
{
    return m_impl->m_terminalDisplay->getVTFont();
}

void QTermWidget::setTerminalOpacity(qreal level)
{
    m_impl->m_terminalDisplay->setOpacity(level);
}

void QTermWidget::setTerminalBackgroundImage(QString backgroundImage)
{
    m_impl->m_terminalDisplay->setBackgroundImage(backgroundImage);
}

void QTermWidget::setShellProgram(const QString &progname)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setProgram(progname);
}

void QTermWidget::setWorkingDirectory(const QString& dir)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setInitialWorkingDirectory(dir);
}

QString QTermWidget::workingDirectory()
{
    if (!m_impl->m_session)
        return QString();

#ifdef Q_OS_LINUX
    // Christian Surlykke: On linux we could look at /proc/<pid>/cwd which should be a link to current
    // working directory (<pid>: process id of the shell). I don't know about BSD.
    // Maybe we could just offer it when running linux, for a start.
    QDir d(QString::fromLatin1("/proc/%1/cwd").arg(getShellPID()));
    if (!d.exists())
    {
        qDebug() << "Cannot find" << d.dirName();
        goto fallback;
    }
    return d.canonicalPath();
#endif

fallback:
    // fallback, initial WD
    return m_impl->m_session->initialWorkingDirectory();
}

void QTermWidget::setArgs(const QStringList &args)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setArguments(args);
}

void QTermWidget::setTextCodec(QTextCodec *codec)
{
    if (!m_impl->m_session)
        return;
    m_impl->m_session->setCodec(codec);
}

void QTermWidget::setColorScheme(const QString& origName)
{
    const ColorScheme *cs = 0;

    const bool isFile = QFile::exists(origName);
    const QString& name = isFile ?
            QFileInfo(origName).baseName() :
            origName;

    // avoid legacy (int) solution
    if (!availableColorSchemes().contains(name))
    {
        if (isFile)
        {
            if (ColorSchemeManager::instance()->loadCustomColorScheme(origName))
                cs = ColorSchemeManager::instance()->findColorScheme(name);
            else
                qWarning () << Q_FUNC_INFO
                        << "cannot load color scheme from"
                        << origName;
        }

        if (!cs)
            cs = ColorSchemeManager::instance()->defaultColorScheme();
    }
    else
        cs = ColorSchemeManager::instance()->findColorScheme(name);

    if (! cs)
    {
        QMessageBox::information(this,
                                 tr("Color Scheme Error"),
                                 tr("Cannot load color scheme: %1").arg(name));
        return;
    }
    ColorEntry table[TABLE_COLORS];
    cs->getColorTable(table);
    m_impl->m_terminalDisplay->setColorTable(table);
}

QStringList QTermWidget::availableColorSchemes()
{
    QStringList ret;
    const auto allColorSchemes = ColorSchemeManager::instance()->allColorSchemes();
    for (const ColorScheme* cs : allColorSchemes) {
        ret.append(cs->name());
        qDebug() << "name:" << cs->name();
    }
    return ret;
}

void QTermWidget::addCustomColorSchemeDir(const QString& custom_dir)
{
    ColorSchemeManager::instance()->addCustomColorSchemeDir(custom_dir);
}

void QTermWidget::setSize(const QSize &size)
{
    m_impl->m_terminalDisplay->setSize(size.width(), size.height());
}

void QTermWidget::setHistorySize(int lines)
{
    if (lines < 0)
        m_impl->m_session->setHistoryType(HistoryTypeFile());
    else
        m_impl->m_session->setHistoryType(HistoryTypeBuffer(lines));
}

void QTermWidget::setScrollBarPosition(ScrollBarPosition pos)
{
    m_impl->m_terminalDisplay->setScrollBarPosition(pos);
}

void QTermWidget::scrollToEnd()
{
    m_impl->m_terminalDisplay->scrollToEnd();
}

void QTermWidget::sendText(const QString &text)
{
    m_impl->m_session->sendText(text);
}

void QTermWidget::resizeEvent(QResizeEvent*)
{
//qDebug("global window resizing...with %d %d", this->size().width(), this->size().height());
    m_impl->m_terminalDisplay->resize(this->size());
}

void QTermWidget::parse(const QByteArray &buf)
{
    QMetaObject::invokeMethod(m_impl->m_session, "onParse", Qt::QueuedConnection, Q_ARG(QByteArray, buf));
}

void QTermWidget::sessionFinished()
{
    emit finished();
}

void QTermWidget::bracketText(QString& text)
{
    m_impl->m_terminalDisplay->bracketText(text);
}

void QTermWidget::copyClipboard()
{
    m_impl->m_terminalDisplay->copyClipboard();
}

void QTermWidget::pasteClipboard()
{
    m_impl->m_terminalDisplay->pasteClipboard();
}

void QTermWidget::pasteSelection()
{
    m_impl->m_terminalDisplay->pasteSelection();
}

void QTermWidget::setZoom(int step)
{
    QFont font = m_impl->m_terminalDisplay->getVTFont();

    font.setPointSize(font.pointSize() + step);
    setTerminalFont(font);
}

void QTermWidget::zoomIn()
{
    setZoom(STEP_ZOOM);
}

void QTermWidget::zoomOut()
{
    setZoom(-STEP_ZOOM);
}

void QTermWidget::setKeyBindings(const QString & kb)
{
    m_impl->m_session->setKeyBindings(kb);
}

void QTermWidget::clear()
{
    m_impl->m_session->emulation()->reset();
    m_impl->m_session->refresh();
    m_impl->m_session->clearHistory();
}

void QTermWidget::setFlowControlEnabled(bool enabled)
{
    m_impl->m_session->setFlowControlEnabled(enabled);
}

QStringList QTermWidget::availableKeyBindings()
{
    return KeyboardTranslatorManager::instance()->allTranslators();
}

QString QTermWidget::keyBindings()
{
    return m_impl->m_session->keyBindings();
}

void QTermWidget::toggleShowSearchBar()
{
    m_searchBar->isHidden() ? m_searchBar->show() : m_searchBar->hide();
}

bool QTermWidget::flowControlEnabled(void)
{
    return m_impl->m_session->flowControlEnabled();
}

void QTermWidget::setFlowControlWarningEnabled(bool enabled)
{
    if (flowControlEnabled()) {
        // Do not show warning label if flow control is disabled
        m_impl->m_terminalDisplay->setFlowControlWarningEnabled(enabled);
    }
}

void QTermWidget::setEnvironment(const QStringList& environment)
{
    m_impl->m_session->setEnvironment(environment);
}

void QTermWidget::setMotionAfterPasting(int action)
{
    m_impl->m_terminalDisplay->setMotionAfterPasting((Konsole::MotionAfterPasting) action);
}

int QTermWidget::historyLinesCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getHistLines();
}

int QTermWidget::screenColumnsCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getColumns();
}

int QTermWidget::screenLinesCount()
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->getLines();
}

void QTermWidget::setSelectionStart(int row, int column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->setSelectionStart(column, row, true);
}

void QTermWidget::setSelectionEnd(int row, int column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->setSelectionEnd(column, row);
}

void QTermWidget::getSelectionStart(int& row, int& column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionStart(column, row);
}

void QTermWidget::getSelectionEnd(int& row, int& column)
{
    m_impl->m_terminalDisplay->screenWindow()->screen()->getSelectionEnd(column, row);
}

QString QTermWidget::selectedText(bool preserveLineBreaks)
{
    return m_impl->m_terminalDisplay->screenWindow()->screen()->selectedText(preserveLineBreaks);
}

void QTermWidget::setMonitorActivity(bool monitor)
{
    m_impl->m_session->setMonitorActivity(monitor);
}

void QTermWidget::setMonitorSilence(bool monitor)
{
    m_impl->m_session->setMonitorSilence(monitor);
}

void QTermWidget::setSilenceTimeout(int seconds)
{
    m_impl->m_session->setMonitorSilenceSeconds(seconds);
}

Filter::HotSpot* QTermWidget::getHotSpotAt(const QPoint &pos) const
{
    int row = 0, column = 0;
    m_impl->m_terminalDisplay->getCharacterPosition(pos, row, column);
    return getHotSpotAt(row, column);
}

Filter::HotSpot* QTermWidget::getHotSpotAt(int row, int column) const
{
    return m_impl->m_terminalDisplay->filterChain()->hotSpotAt(row, column);
}

QList<QAction*> QTermWidget::filterActions(const QPoint& position)
{
    return m_impl->m_terminalDisplay->filterActions(position);
}

int QTermWidget::getPtySlaveFd() const
{
    return m_impl->m_session->getPtySlaveFd();
}

void QTermWidget::setKeyboardCursorShape(KeyboardCursorShape shape)
{
    m_impl->m_terminalDisplay->setKeyboardCursorShape(shape);
}

void QTermWidget::setBlinkingCursor(bool blink)
{
    m_impl->m_terminalDisplay->setBlinkingCursor(blink);
}

void QTermWidget::setBidiEnabled(bool enabled)
{
    m_impl->m_terminalDisplay->setBidiEnabled(enabled);
}

bool QTermWidget::isBidiEnabled()
{
    return m_impl->m_terminalDisplay->isBidiEnabled();
}

QString QTermWidget::title() const
{
    QString title = m_impl->m_session->userTitle();
    if (title.isEmpty())
        title = m_impl->m_session->title(Konsole::Session::NameRole);
    return title;
}

QString QTermWidget::icon() const
{
    QString icon = m_impl->m_session->iconText();
    if (icon.isEmpty())
        icon = m_impl->m_session->iconName();
    return icon;
}

bool QTermWidget::isTitleChanged() const
{
    return m_impl->m_session->isTitleChanged();
}

void QTermWidget::setAutoClose(bool autoClose)
{
    m_impl->m_session->setAutoClose(autoClose);
}

void QTermWidget::cursorChanged(Konsole::Emulation::KeyboardCursorShape cursorShape, bool blinkingCursorEnabled)
{
    // TODO: A switch to enable/disable DECSCUSR?
    setKeyboardCursorShape(cursorShape);
    setBlinkingCursor(blinkingCursorEnabled);
}

void QTermWidget::setMargin(int margin)
{
    m_impl->m_terminalDisplay->setMargin(margin);
}

int QTermWidget::getMargin() const
{
    return m_impl->m_terminalDisplay->margin();
}
