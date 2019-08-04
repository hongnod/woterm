#include "qwosessionmanage.h"
#include "ui_qwosessionmanage.h"
#include "qwohostsimplelist.h"
#include "qtermwidget.h"

#include "qwosetting.h"
#include "qwoutils.h"
#include "qwosshconf.h"

#include <QFileDialog>
#include <QDebug>
#include <QIntValidator>
#include <QStringListModel>
#include <QMessageBox>

QWoSessionManage::QWoSessionManage(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoSessionManage)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    setWindowTitle(tr("Session Property"));

    ui->tableView->setModel(&m_model);

}

QWoSessionManage::~QWoSessionManage()
{
    delete ui;
}
