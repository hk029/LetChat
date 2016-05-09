#include "multichatdlg.h"
#include "ui_multichatdlg.h"

MultiChatDlg::MultiChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiChatDlg)
{
    ui->setupUi(this);
}

MultiChatDlg::~MultiChatDlg()
{
    delete ui;
}
