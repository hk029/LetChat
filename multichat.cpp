#include "multichat.h"
#include "ui_multichat.h"

MultiChat::MultiChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiChat)
{
    ui->setupUi(this);
    this->wdg = new Widget();
    this->ui->horizontalLayout->addWidget(this->wdg);
    this->wdg->show();
}

MultiChat::~MultiChat()
{
    delete ui;
}
