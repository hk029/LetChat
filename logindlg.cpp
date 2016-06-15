#include "logindlg.h"
#include "ui_logindlg.h"

LoginDlg::LoginDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDlg)
{
    ui->setupUi(this);
    this->name = "";
    bkg.load(":/img/bkg_main.png");
    //*********************标题栏*********************//
    QIcon icon;
    QPixmap icon_close(":/img/icon_close.png");
    QPixmap icon_mini(":/img/icon_mini.png");
    icon_close.scaled(this->ui->icon_close->size());
    icon_mini.scaled(this->ui->icon_mini->size());
    icon.addPixmap(icon_close);
    this->ui->icon_close->setIcon(icon);
    icon.addPixmap(icon_mini);
    this->ui->icon_mini->setIcon(icon);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

    QFile qssfile(":/style/base.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);

}

LoginDlg::~LoginDlg()
{
    delete ui;
}

 QString LoginDlg::getName()
 {
     return this->name;
 }


void LoginDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}

void LoginDlg::on_icon_close_clicked()
{
    this->close();
}

void LoginDlg::on_pushButton_clicked()
{
    QString tmp = this->ui->nameedit->text();
    qDebug()<<tmp;
    if(tmp == "")
        QMessageBox::warning(this, "错误", QString::fromLocal8Bit("姓名不能为空"), "好的");
    else
    {
        this->name = tmp;
        this->close();
    }

}

void LoginDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // 绘制不规则背景

}

void LoginDlg::mousePressEvent(QMouseEvent *event){
        QPoint windowPos = this->pos();
        QPoint mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;

}
void LoginDlg::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}

