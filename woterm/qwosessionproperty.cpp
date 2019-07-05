#include "qwosessionproperty.h"
#include "ui_qwosessionproperty.h"

QWoSessionProperty::QWoSessionProperty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QWoSessionProperty)
{
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags &~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    ui->tree->setModel(&m_model);
    ui->tree->setIndentation(10);
    QStandardItem *itemA = new QStandardItem("A");
    m_model.appendRow(itemA);
    itemA->appendRow(new QStandardItem("A1"));
    itemA->appendRow(new QStandardItem("A2"));

    QStandardItem *itemB = new QStandardItem("B");
    itemB->appendRow(new QStandardItem("B1"));
    itemB->appendRow(new QStandardItem("B2"));
    m_model.appendRow(itemB);


    QStandardItem *itemC = new QStandardItem("C");
    itemC->appendRow(new QStandardItem("C1"));
    itemC->appendRow(new QStandardItem("C2"));
    m_model.appendRow(itemC);
}

QWoSessionProperty::~QWoSessionProperty()
{
    delete ui;
}
