#include "qwohostsimplelist.h"
#include "ui_qwohostlist.h"

QWoHostSimpleList::QWoHostSimpleList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QWoHostList)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

QWoHostSimpleList::~QWoHostSimpleList()
{

}
