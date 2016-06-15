#include "privatechatdlg.h"
#include "ui_privatechatdlg.h"

PrivateChatDlg::PrivateChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateChatDlg)
{
    ui->setupUi(this);
    //socket = new QUdpSocket(this);
    //socket->bind(PORT,QUdpSocket::ShareAddress);
     //connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));
     // connect(this->parent(),SIGNAL(mysignal()),this,SLOT(processPendingDatagram()));
      this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
      this->setAttribute(Qt::WA_TranslucentBackground);
       bkg.load(":/img/bkg.png");
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
      this->setStyleSheet("QLabel{"
                           "color:#4a4a4a;"
                            "}"

                           "QPushButton{"
                           "background-color:#666666;color:white;"
                           "border:0px;"
                           "}"
                           "QTextBrowser{"
                           "color:red;"
                           "}"
                           "QToolButton{"
                           "border:0px;"
                           "}"
                           "QToolButton:hover{"
                           "background-color:#ff5400;"
                           "}"
                           "QToolButton:pressed{"
                           "background-color:#fff;"
                           "}"
                           "QPushButton:hover{"
                           "background-color:#D35400;"
                           "}"
                           );
}
PrivateChatDlg::PrivateChatDlg(QUdpSocket *s)
{
    this->socket = s;
}

PrivateChatDlg::~PrivateChatDlg()
{
    delete ui;
}


/**
 * MakeMsg
 * \brief   构造消息函数
 * \param   str 需要发送的字符串
 * \param   type 构造类型，根据MsgType
 * \return  构造好的消息
 * \update
    16-5-18: 新增
    16-5-23：修改上线消息，解决用户上线互相发送信息的死循环
 */

QString PrivateChatDlg::MakeMsg(QString str,int type)
{
    char header = 0x01;
    QByteArray bytes;
    bytes.append(header);
    switch(type)
    {
        case PRITEXT:
        //****************************************
        // |0x01|0x04|name_len|name|str|
        // |私聊消息| name长度 | name| str|
        //****************************************
        bytes.append(0x04);
        bytes.append(this->name.length());
        bytes.append(this->name);
        bytes.append(str);
        break;

    }
    return QString(bytes);
}

/**
 * ResolveMsg
 * \brief   解析函数
 * \param   bytes 需要解析的字符串
 * \return  如果没正确解析返回-1
 * \update
    16-5-18: 新增
    16-5-23：修改下线消息，解决删除不在列表中的用户造成的死机

 */

int PrivateChatDlg::ResolveMsg(QByteArray bytes)
{
    unsigned char header = bytes[0];
    QString ip,name,msg;
    int len,lenName;
    unsigned char type = bytes[1];
    //*****chengcheng*****
    int nameStartIndex = 0;
    QList<QStandardItem *> tList ;
    QStandardItem* tItem ;

    //********************添加变量×××××××××××××××××××
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString reTime = time.toString("hh:mm"); //设置显示格式
    //********************
    int row,rows;
    switch((int)type)
    {
        case PRITEXT:
        lenName = bytes[2];
        qDebug()<<lenName;
        name = bytes.mid(3,lenName);
        msg = bytes.mid(3+lenName,(bytes.length()-(3+lenName)));
        //**************2016.5.22****修改****
        //判断输入是否为一串空格，若是，则弹出提示××××××××××××××××××××
        if(msg.trimmed().length() == 0){
            QMessageBox::warning(this, "错误", QString::fromLocal8Bit("发送的内容不能为空"), "好的");
            break;
        }
        if(name == this->name){
            this->ui->receiveMsg->setTextColor("red");
        }
        else{
            this->ui->receiveMsg->setTextColor("blue");
        }
        //********************
        //time = QDateTime::currentDateTime();//获取系统现在的时间
        //reTime = time.toString("hh:mm"); //设置显示格式
        qDebug()<<reTime;
        this->ui->receiveMsg->append(name+" "+reTime+":\n"+msg);
        break;
    default:
        return -1;
        break;
    }
    return 1;
}


/**
 * paintEvent
 * \brief   处理等待的数据报
 * \return  null
 * \update
    16-5-13: 重绘透明背景
 */

void PrivateChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // 绘制不规则背景

}


void PrivateChatDlg::processPendingDatagram() //
{
    qDebug()<<"111";
    while(socket->hasPendingDatagrams())  //拥有等待的数据报
    {

        QByteArray datagram; //拥于存放接收的数据报
        //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        datagram.resize(socket->pendingDatagramSize());
        //接收数据报，将其存放到datagram中
        socket->readDatagram(datagram.data(),datagram.size());
        //将数据报内容显示出来
        this->ResolveMsg(datagram);
    }
}

 void PrivateChatDlg::setName(QString n,QString oth)
{
     this->name = n;
     this->OtherName = oth;
     this->refreshtext();
 }
 void PrivateChatDlg::setSocket(QUdpSocket *s)
{
     this->socket = s;
 }
QString PrivateChatDlg::getName()
{
    return this->name;
}

 void PrivateChatDlg::refreshtext()
{
    //QString title;
    // title.sprintf("与%s聊天中....",this->OtherName);
    this->ui->title->setText("与 [" + this->OtherName + "] 聊天中...");
 }

 bool PrivateChatDlg::setHostIP(QString host)
 {
     return this->hostIP = new QHostAddress(host);
 }

 /**
  * on_icon_close_clicked
  * \brief   如果关闭按钮按下
  * \return  null
  * \update
     16-5-14: 新增
     16-5-23：关闭发送下线信息
  */
 void PrivateChatDlg::on_icon_close_clicked()
 {
   //  emit closedlg();
     this->name = "";
     this->close();
 }

 /**
  * mousePressEvent
  * \brief   如果鼠标按下准备拖拽窗口
  * \return  null
  * \update
     16-5-14: 新增
  */
 void PrivateChatDlg::mousePressEvent(QMouseEvent *event){
         QPoint windowPos = this->pos();
         QPoint mousePos = event->globalPos();
         this->dPos = mousePos - windowPos;

 }


 /**
  * mouseMoveEvent
  * \brief   如果拖拽窗口
  * \return  null
  * \update
     16-5-14: 新增
  */
 void PrivateChatDlg::mouseMoveEvent(QMouseEvent *event){
         this->move(event->globalPos() - this->dPos);
 }
 /**
  * on_icon_mini_clicked
  * \brief   如果最小化按钮按下
  * \return  null
  * \update
     16-5-14: 新增
  */



void PrivateChatDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}

void PrivateChatDlg::on_sendMsg_textChanged()
{
      bool bool_dis = this->ui->sendMsg->toPlainText().isEmpty();
     // this->ui->sendButton->setDisabled(bool_dis);
      if(!bool_dis)
      {
          ui->sendButton->setStyleSheet("background-color:#3dce3d");//green
          this->ui->sendButton->setDisabled(bool_dis);
      }
      else{
          ui->sendButton->setStyleSheet("background-color:#666");//灰色
          this->ui->sendButton->setDisabled(bool_dis);
      }
}

void PrivateChatDlg::on_sendButton_clicked()
{
    QString str =  this->ui->sendMsg->toPlainText();
    str = this->MakeMsg(str,PRITEXT);
   this->SendMsg(str,*this->hostIP);
}

int PrivateChatDlg::SendMsg(QString str,QHostAddress host)
{
    QByteArray data = str.toAscii();
    this->ui->sendMsg->clear();
    return socket->writeDatagram(data,QHostAddress::Broadcast,PORT);
}



