#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"
#include "qwohostsimplelist.h"
#include "qtermwidget.h"

#include "qwosetting.h"
#include "qwoutils.h"

#include <QFileDialog>
#include <QDebug>
#include <QIntValidator>
#include <QStringListModel>

QWoSessionProperty::QWoSessionProperty(int type, QWidget *parent)
    : QDialog(parent)
    , m_type(type)
    , ui(new Ui::QWoSessionProperty)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Session Property"));

    m_modelPreview = new QStringListModel(this);
    ui->schema->setModel(m_modelPreview);
    m_preview = new QTermWidget(this);
    QStringList colors = m_preview->availableColorSchemes();
    m_modelPreview->setStringList(colors);
    ui->previewLayout->addWidget(m_preview);
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start(1000);

    QObject::connect(ui->schema, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onColorCurrentIndexChanged(const QString &)));

    ui->port->setValidator(new QIntValidator(1, 65535));
    ui->lineSize->setValidator(new QIntValidator(DEFAULT_HISTORY_LINE_LENGTH, 65535));

    ui->fontChooser->setEditable(false);
    ui->fontChooser->setFontFilters(QFontComboBox::MonospacedFonts);
    QObject::connect(ui->fontChooser, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(onCurrentFontChanged(const QFont&)));

    ui->tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tree->setModel(&m_model);
    ui->tree->setIndentation(10);
    QStandardItem *connect = new QStandardItem(tr("Connect"));
    m_model.appendRow(connect);
    connect->appendRow(new QStandardItem(tr("Authentication")));
    QStandardItem *terminal = new QStandardItem(tr("Terminal"));
    m_model.appendRow(terminal);
    QStandardItem *appearance = new QStandardItem(tr("Appearance"));
    m_model.appendRow(appearance);
    QStandardItem *fileTransfre = new QStandardItem(tr("FileTransfer"));
    m_model.appendRow(fileTransfre);
    QObject::connect(ui->tree, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTreeItemClicked(const QModelIndex&)));

    QObject::connect(ui->blockCursor, SIGNAL(toggled(bool)), this, SLOT(onBlockCursorToggled()));
    QObject::connect(ui->underlineCursor, SIGNAL(toggled(bool)), this, SLOT(onUnderlineCursorToggled()));
    QObject::connect(ui->beamCursor, SIGNAL(toggled(bool)), this, SLOT(onBeamCursorToggled()));

    QObject::connect(ui->authType, SIGNAL(currentIndexChanged(const QString &)),  this, SLOT(onAuthCurrentIndexChanged(const QString &)));
    QObject::connect(ui->connect, SIGNAL(clicked()), this, SLOT(onReadyToConnect()));
    QObject::connect(ui->save, SIGNAL(clicked()), this, SLOT(onReadyToSave()));
    QObject::connect(ui->cancel, SIGNAL(clicked()), this, SLOT(close()));

    QObject::connect(ui->szDirBrowser, SIGNAL(clicked()), this, SLOT(onSzDirBrowser()));
    QObject::connect(ui->rzDirBrowser, SIGNAL(clicked()), this, SLOT(onRzDirBrowser()));

    QObject::connect(ui->identifyBrowser, SIGNAL(clicked()),  this, SLOT(onIdentifyBrowserClicked()));
    QObject::connect(ui->jumpBrowser, SIGNAL(clicked()),  this, SLOT(onJumpBrowserClicked()));

    QObject::connect(ui->fontSize, SIGNAL(valueChanged(int)), this, SLOT(onFontValueChanged(int)));

    initHistory();
    initDefault();


    if(m_type == SPTYPE_DEFAULT) {
        ui->connect->hide();
    }
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}

void QWoSessionProperty::onAuthCurrentIndexChanged(const QString &txt)
{
    bool isPass = txt == tr("Password");

    QWoUtils::setLayoutVisible(ui->passLayout, isPass);
    QWoUtils::setLayoutVisible(ui->identifyLayout, !isPass);
}

/*
Ps = 3 0  -> Set foreground color to Black.
            Ps = 3 1  -> Set foreground color to Red.
            Ps = 3 2  -> Set foreground color to Green.
            Ps = 3 3  -> Set foreground color to Yellow.
            Ps = 3 4  -> Set foreground color to Blue.
            Ps = 3 5  -> Set foreground color to Magenta.
            Ps = 3 6  -> Set foreground color to Cyan.
            Ps = 3 7  -> Set foreground color to White.
            Ps = 3 9  -> Set foreground color to default, ECMA-48 3rd.
            Ps = 4 0  -> Set background color to Black.
            Ps = 4 1  -> Set background color to Red.
            Ps = 4 2  -> Set background color to Green.
            Ps = 4 3  -> Set background color to Yellow.
            Ps = 4 4  -> Set background color to Blue.
            Ps = 4 5  -> Set background color to Magenta.
            Ps = 4 6  -> Set background color to Cyan.
            Ps = 4 7  -> Set background color to White.
            Ps = 4 9  -> Set background color to default, ECMA-48 3rd.
*/
void QWoSessionProperty::onColorCurrentIndexChanged(const QString &txt)
{
    m_preview->setColorScheme(txt);
}

void QWoSessionProperty::onCurrentFontChanged(const QFont &font)
{
    QFont f = font;
    f.setPointSize(ui->fontSize->value());
    m_preview->setTerminalFont(f);
    m_preview->setBlinkingCursor(true);
}

void QWoSessionProperty::onBlockCursorToggled()
{
    m_preview->setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::BlockCursor);
    m_preview->setBlinkingCursor(true);
}

void QWoSessionProperty::onUnderlineCursorToggled()
{
    m_preview->setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::UnderlineCursor);
    m_preview->setBlinkingCursor(true);
}

void QWoSessionProperty::onBeamCursorToggled()
{
    m_preview->setKeyboardCursorShape(Konsole::Emulation::KeyboardCursorShape::IBeamCursor);
    m_preview->setBlinkingCursor(true);
}

void QWoSessionProperty::onFontValueChanged(int i)
{
    QFont f = m_preview->getTerminalFont();
    f.setPointSize(i);
    m_preview->setTerminalFont(f);
}

void QWoSessionProperty::onTreeItemClicked(const QModelIndex &idx)
{
    QString name = idx.data().toString();
    if(name == tr("Connect")) {
        ui->stacked->setCurrentWidget(ui->connectWidget);
    }else if(name == tr("Authentication")){
        ui->stacked->setCurrentWidget(ui->authenticationWidget);
    }else if(name == tr("Terminal")){
        ui->stacked->setCurrentWidget(ui->terminalWidget);
    }else if(name == tr("Appearance")){
        ui->stacked->setCurrentWidget(ui->appearanceWidget);
    }else if(name == tr("FileTransfer")){
        ui->stacked->setCurrentWidget(ui->fileTransferWidget);
    }
}

void QWoSessionProperty::onIdentifyBrowserClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"));
    qDebug() << "fileName" << fileName;
    fileName = QDir::toNativeSeparators(fileName);
    ui->identify->setCurrentText(fileName);
}

void QWoSessionProperty::onJumpBrowserClicked()
{
    QWoHostSimpleList dlg(this);
    dlg.exec();
    HostInfo hi;
    if(dlg.result(&hi)) {
        ui->jump->setEditText(hi.name);
    }
}

void QWoSessionProperty::onReadyToConnect()
{

}

void QWoSessionProperty::onReadyToSave()
{

}

void QWoSessionProperty::onSzDirBrowser()
{
    QString path = ui->szUpload->text();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly);
    qDebug() << "fileName" << fileName;
    if(!fileName.isEmpty()) {
        fileName = QDir::toNativeSeparators(fileName);
        ui->szUpload->setText(fileName);
    }
}

void QWoSessionProperty::onRzDirBrowser()
{
    QString path = ui->rzDown->text();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly);
    qDebug() << "fileName" << fileName;
    if(!fileName.isEmpty()) {
        fileName = QDir::toNativeSeparators(fileName);
        ui->rzDown->setText(fileName);
    }
}

void QWoSessionProperty::initDefault()
{
    QDir home = QDir::home();
    QVariant val = QWoSetting::value("property/default");
    QVariantMap mdata = val.toMap();
    QString szPathUpload = mdata.value("zmodemPathUpload", QDir::toNativeSeparators(home.path())).toString();
    QString rzPathDown = mdata.value("zmodemPathDown", QDir::toNativeSeparators(home.path())).toString();
    ui->szUpload->setText(szPathUpload);
    ui->rzDown->setText(rzPathDown);
    ui->port->setText(mdata.value("port", 22).toString());
    QString password = mdata.value("password").toString();
    QString identify = mdata.value("identifyFIle").toString();
    ui->password->setText(password);
    ui->identify->setCurrentText(identify);
    if(!password.isEmpty() || identify.isEmpty()) {
        ui->authType->setCurrentText("Password");
        onAuthCurrentIndexChanged("Password");
    }else if(!identify.isEmpty()){
        ui->authType->setCurrentText("IdentifyFile");
        onAuthCurrentIndexChanged("IdentifyFile");
    }
    QString userName = mdata.value("userName").toString();
    ui->userName->setEditText(userName);
    QString schema = mdata.value("colorSchema", DEFAULT_COLOR_SCHEMA).toString();
    ui->schema->setCurrentText(schema);
    QString binding = mdata.value("keyBinding", DEFAULT_KEYBOARD_BINDING).toString();
    QString fontName = mdata.value("fontName", DEFAULT_FONT_FAMILY).toString();
    QFont font(fontName);
    ui->fontChooser->setCurrentFont(font);

    QString cursorName = mdata.value("cursorName", "block").toString();
    if(cursorName.isEmpty() || cursorName == "block") {
        ui->blockCursor->setChecked(true);
    }else if(cursorName == "underline") {
        ui->underlineCursor->setChecked(true);
    }else {
        ui->beamCursor->setChecked(true);
    }
    QString line = mdata.value("historyLine", QString("%1").arg(DEFAULT_HISTORY_LINE_LENGTH)).toString();
    ui->lineSize->setText(line);
}

void QWoSessionProperty::initHistory()
{
    {
        QVariant v = QWoSetting::value("history/identifyList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->identify->setModel(model);
    }
    {
        QVariant v = QWoSetting::value("history/userNameList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->userName->setModel(model);
    }
    {
        QVariant v = QWoSetting::value("history/proxyJumpList");
        QStringList el = v.toStringList();
        QStringListModel *model = new QStringListModel(this);
        model->setStringList(el);
        ui->jump->setModel(model);
    }
}

void QWoSessionProperty::saveDefaultConfig()
{

}

void QWoSessionProperty::setFixPreviewString()
{
    m_preview->clear();
    QByteArray seqTxt = "\033[31mRed \033[32mGreen \033[33mYellow \033[34mBlue";
    seqTxt.append("\r\n\033[35mMagenta \033[36mCyan \033[37mWhite \033[39mDefault");
    seqTxt.append("\r\n\033[40mBlack \033[41mRed \033[42mGreen \033[43mYellow \033[44mBlue");
    seqTxt.append("\r\n\033[45mMagenta \033[46mCyan \033[47mWhite \033[49mDefault");
    m_preview->parseSequenceText(seqTxt);
    m_preview->setBlinkingCursor(true);
}

void QWoSessionProperty::onTimeout()
{
    setFixPreviewString();
}
