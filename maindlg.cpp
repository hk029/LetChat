#include "maindlg.h"
#include "ui_maindlg.h"

MainDlg::MainDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDlg)
{
    ui->setupUi(this);
    bkg.load(":/img/bkg_main.png");

    //*********************������*********************//
    QIcon icon;
    QPixmap icon_close(":/img/icon_close.png");
    QPixmap icon_mini(":/img/icon_mini.png");
    icon_close.scaled(this->ui->icon_close->size());
    icon_mini.scaled(this->ui->icon_mini->size());
    icon.addPixmap(icon_close);
    this->ui->icon_close->setIcon(icon);
    icon.addPixmap(icon_mini);
    this->ui->icon_mini->setIcon(icon);

    //*********************ͷ��*********************//
    QPixmap ph(":/img/photo.png");
    ph.scaled(500, 500).scaled(60,60, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);



    this->ui->photo->setPixmap(ph);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->multidlg = new MultiChatDlg();
    this->setStyleSheet("#photo{"
                        "border:1px;"
                        "}"
                          "QLabel{"
                          "color:#4a4a4a;"
                           "}"

                          "QPushButton{"
                          "background-color:#3dce3d;color:white;"
                          "border:0px;"
                          "}"
                          "QTextBrowser{"
                          "color:red;"
                          "}"
                          "QToolButton{"
                          "border:1px;"
                          "}"

                          "QToolButton:hover{"
                          "background-color:#ff2200;"
                          "}"
                          "QPushButton:hover{"
                          "background-color:#D35400;"
                          "}"
                          );
}

MainDlg::~MainDlg()
{
    delete ui;
}


void MainDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // ���Ʋ����򱳾�

}

void MainDlg::on_icon_close_clicked()
{
    this->close();
}

void MainDlg::on_IntoChatRoom_clicked()
{
    this->multidlg->show();
}
void MainDlg::mousePressEvent(QMouseEvent *event){
        QPoint windowPos = this->pos();
        QPoint mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;

}
void MainDlg::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}

void MainDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}
