#include "qwopasswordinput.h"
#include "ui_qwopasswordinput.h"

QWoPasswordInput::QWoPasswordInput(const QString &prompt, bool echo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QWoPasswordInput)
{
    ui->setupUi(this);
    ui->tip->setText(prompt);
    if(!echo) {
        ui->visible->show();
        ui->password->setEchoMode(QLineEdit::Password);
    }else{
        ui->visible->hide();
    }

    QObject::connect(ui->visible, SIGNAL(clicked(bool)), this, SLOT(onPasswordVisible(bool)));
    QObject::connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(onClose()));

    Qt::WindowFlags flags = windowFlags();
    flags = flags &~Qt::WindowContextHelpButtonHint;
    flags &= Qt::FramelessWindowHint;
    setWindowFlags(flags);

    QPalette pal;
    pal.setColor(QPalette::Background, QColor(Qt::lightGray));
    setPalette(pal);
    setAutoFillBackground(true);

    setAttribute(Qt::WA_DeleteOnClose);
}

QWoPasswordInput::~QWoPasswordInput()
{
    delete ui;
}

QString QWoPasswordInput::result() const
{
    return m_result;
}

void QWoPasswordInput::onPasswordVisible(bool checked)
{
    ui->password->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
}

void QWoPasswordInput::onClose()
{
    m_result = ui->password->text();
    close();
}
