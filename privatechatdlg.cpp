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
 * \brief   ������Ϣ����
 * \param   str ��Ҫ���͵��ַ���
 * \param   type �������ͣ�����MsgType
 * \return  ����õ���Ϣ
 * \update
    16-5-18: ����
    16-5-23���޸�������Ϣ������û����߻��෢����Ϣ����ѭ��
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
        // |˽����Ϣ| name���� | name| str|
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
 * \brief   ��������
 * \param   bytes ��Ҫ�������ַ���
 * \return  ���û��ȷ��������-1
 * \update
    16-5-18: ����
    16-5-23���޸�������Ϣ�����ɾ�������б��е��û���ɵ�����

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

    //********************��ӱ�����������������������������������������
    QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
    QString reTime = time.toString("hh:mm"); //������ʾ��ʽ
    //********************
    int row,rows;
    switch((int)type)
    {
        case PRITEXT:
        lenName = bytes[2];
        qDebug()<<lenName;
        name = bytes.mid(3,lenName);
        msg = bytes.mid(3+lenName,(bytes.length()-(3+lenName)));
        //**************2016.5.22****�޸�****
        //�ж������Ƿ�Ϊһ���ո����ǣ��򵯳���ʾ����������������������������������������
        if(msg.trimmed().length() == 0){
            QMessageBox::warning(this, "����", QString::fromLocal8Bit("���͵����ݲ���Ϊ��"), "�õ�");
            break;
        }
        if(name == this->name){
            this->ui->receiveMsg->setTextColor("red");
        }
        else{
            this->ui->receiveMsg->setTextColor("blue");
        }
        //********************
        //time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
        //reTime = time.toString("hh:mm"); //������ʾ��ʽ
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
 * \brief   ����ȴ������ݱ�
 * \return  null
 * \update
    16-5-13: �ػ�͸������
 */

void PrivateChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // ���Ʋ����򱳾�

}


void PrivateChatDlg::processPendingDatagram() //
{
    qDebug()<<"111";
    while(socket->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {

        QByteArray datagram; //ӵ�ڴ�Ž��յ����ݱ�
        //��datagram�Ĵ�СΪ�ȴ���������ݱ��Ĵ�С���������ܽ��յ�����������
        datagram.resize(socket->pendingDatagramSize());
        //�������ݱ��������ŵ�datagram��
        socket->readDatagram(datagram.data(),datagram.size());
        //�����ݱ�������ʾ����
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
    // title.sprintf("��%s������....",this->OtherName);
    this->ui->title->setText("�� [" + this->OtherName + "] ������...");
 }

 bool PrivateChatDlg::setHostIP(QString host)
 {
     return this->hostIP = new QHostAddress(host);
 }

 /**
  * on_icon_close_clicked
  * \brief   ����رհ�ť����
  * \return  null
  * \update
     16-5-14: ����
     16-5-23���رշ���������Ϣ
  */
 void PrivateChatDlg::on_icon_close_clicked()
 {
   //  emit closedlg();
     this->name = "";
     this->close();
 }

 /**
  * mousePressEvent
  * \brief   �����갴��׼����ק����
  * \return  null
  * \update
     16-5-14: ����
  */
 void PrivateChatDlg::mousePressEvent(QMouseEvent *event){
         QPoint windowPos = this->pos();
         QPoint mousePos = event->globalPos();
         this->dPos = mousePos - windowPos;

 }


 /**
  * mouseMoveEvent
  * \brief   �����ק����
  * \return  null
  * \update
     16-5-14: ����
  */
 void PrivateChatDlg::mouseMoveEvent(QMouseEvent *event){
         this->move(event->globalPos() - this->dPos);
 }
 /**
  * on_icon_mini_clicked
  * \brief   �����С����ť����
  * \return  null
  * \update
     16-5-14: ����
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
          ui->sendButton->setStyleSheet("background-color:#666");//��ɫ
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



