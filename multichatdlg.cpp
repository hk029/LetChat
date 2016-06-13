#include "multichatdlg.h"
#include "ui_multichatdlg.h"
//#include <QStandardItemModel>

#include <QDateTime>
MultiChatDlg::MultiChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiChatDlg)
{
    ui->setupUi(this);
    this->pdlg = NULL;
    //*********************����event filter*********************//
    this->installEventFilter(this);
    name = "hk";
    hostIP = new QHostAddress(QHostAddress::Broadcast);
    socket = new QUdpSocket(this);
    socket->bind(PORT,QUdpSocket::ShareAddress);
    connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));

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

    QFile qssfile(":/style/base.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);

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
//    tcpSocket = new QTcpSocket(this);
//    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
//    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
    numOfOnline = -1;


    //******************chengcheng******************************
    //1.��ӱ�ͷ
    model = new QStandardItemModel();

    model->setColumnCount(2);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("name"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Ip address"));

    //2.���ñ������
    ui->contactList->setModel(model);
    //��ͷ��Ϣ��ʾ����
    ui->contactList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //�����п��ɱ�
    ui->contactList->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    ui->contactList->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->contactList->setColumnWidth(0,150);
    ui->contactList->setColumnWidth(1,100);
    //3.�����
//    for(int i = 0; i<3; i++){
//        model->setItem(i,0,new QStandardItem("11111"));
//        //������ɫ
//        model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//        //�����ַ�λ��
//        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
//        model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//    }

    QString IP = this->getIP();
    QString s = this->MakeMsg(IP+'/'+this->name,ONLINE);
    this->SendMsg(s,QHostAddress::Broadcast);
//    int row = tItem->row();

    //************************��ʼ����Ϊ��,���Ͱ�ť������
    //bool bool_dis = this->ui->sendMsg->toPlainText().isEmpty();
    //this->ui->sendButton
    //this->ui->sendButton->setDisabled(bool_dis);

}

MultiChatDlg::~MultiChatDlg()
{
    delete ui;
}

//*********************add change_btn_stat()
//void MultiChatDlg::change_btn_stat()
//{
 //    bool bool_dis = this->ui->sendMsg->toPlainText().isEmpty();
  //   this->ui->sendButton->setDisabled(bool_dis);
//}


/**
 * paintEvent
 * \brief   ����ȴ������ݱ�
 * \return  null
 * \update
    16-5-13: �ػ�͸������
 */

void MultiChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // ���Ʋ����򱳾�

}

/**
 * getIP
 * \brief   ��ȡ��ǰ������ip
 * \return  null
 * \update
    16-5-14: ����
    16-5-23���޸�Ϊ����ȷ��ȡ��ǰʹ�õ�ip
 */

QString MultiChatDlg::getIP()  //��ȡip��ַ
{
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();

     foreach(QNetworkInterface interfaceItem, interfaceList)
     {
         if(interfaceItem.flags().testFlag(QNetworkInterface::IsUp)
                 &&interfaceItem.flags().testFlag(QNetworkInterface::IsRunning)
                 &&interfaceItem.flags().testFlag(QNetworkInterface::CanBroadcast)
                 &&interfaceItem.flags().testFlag(QNetworkInterface::CanMulticast)
                 &&!interfaceItem.flags().testFlag(QNetworkInterface::IsLoopBack)
                 &&interfaceItem.hardwareAddress()!="00:50:56:C0:00:01"
                 &&interfaceItem.hardwareAddress()!="00:50:56:C0:00:08")
         {
             QList<QNetworkAddressEntry> addressEntryList=interfaceItem.addressEntries();
             foreach(QNetworkAddressEntry addressEntryItem, addressEntryList)
             {
                 if(addressEntryItem.ip().protocol()==QAbstractSocket::IPv4Protocol)
                 {
                     return addressEntryItem.ip().toString();
                 }
             }
         }
     }
     return NULL;
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

QString MultiChatDlg::MakeMsg(QString str,int type)
{
    char header = 0x01;
    QByteArray bytes;
    bytes.append(header);
    QString ip = this->getIP();
    switch(type)
    {
        case ONLINE:
        //****************************************
        // |0x01|0x01|ip_len|ip_str|name_len|name|
        // |������Ϣ| ip����(byte)|ip | name����(byte) | name|
        //****************************************
        bytes.append(0x01);
        bytes.append(ip.length());
        bytes.append(ip);
        bytes.append(this->name.length());
        bytes.append(this->name);
        break;
        case OFFLINE:
        //****************************************
        // |0x01|0x02|ip_len|ip_str|name_len|name|
        // |������Ϣ| ip����(byte)|ip | name����(byte) | name|
        //****************************************
        bytes.append(0x02);
        bytes.append(ip.length());
        bytes.append(ip);
        bytes.append(name.length());
        bytes.append(name);
        break;
        case MULTEXT:
        //****************************************
        // |0x01|0x03|name_len|name|str|
        // |��ͨ��Ϣ| name���� | name| str|
        //****************************************
        bytes.append(0x03);
        bytes.append(this->name.length());
        bytes.append(this->name);
        bytes.append(str);
        qDebug()<<bytes;   
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

int MultiChatDlg::ResolveMsg(QByteArray bytes)
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
    //QDateTime time;
    //QString reTime;
    QDateTime time = QDateTime::currentDateTime();//��ȡϵͳ���ڵ�ʱ��
    QString reTime = time.toString("hh:mm"); //������ʾ��ʽ
    //********************
    int row,rows;
    switch((int)type)
    {
        case ONLINE:
        len = bytes[2];
        //get ip
        //*************chengcheng************
        len = bytes[2];
        ip = bytes.mid(3,len);
        nameStartIndex = 2+len+1+1;
        len = bytes[2+len+1];
        name = bytes.mid(nameStartIndex,len);
        //�ڱ������ӵ�¼״̬
        numOfOnline++;
        model->appendRow(new QStandardItem(name));
        rows= model->rowCount();
       // model->setItem(numOfOnline,0,new QStandardItem(ip));
        //������ɫ
        model->item(rows-1,0)->setForeground(QBrush(QColor(255,0,0)));
        //�����ַ�λ��
        model->item(rows-1,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(rows-1,1,new QStandardItem(ip));
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"] "+reTime+" ������...");

        //update 5-23
        if(this->getIP() != ip && model->findItems(ip).empty())
            this->SendMsg(this->MakeMsg("",ONLINE),QHostAddress(ip));
        break;

        case OFFLINE:
        //************************chengcheng*********************************
        len = bytes[2];
        ip = bytes.mid(3,len);
        nameStartIndex = 2+len+1+1;
        len = bytes[2+len+1];

        name = bytes.mid(nameStartIndex,len);
        tList = model->findItems(ip);//ָ��������
        //update 5-23
        if(!tList.empty()){
            tItem = tList.at(0);//���յ�һ�е�ֵ����

            row = tItem->row();

            model->removeRow(row);//�Ƴ�
        }


        //*******************************************************************
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"] " + reTime+" ������...");
        break;

        case MULTEXT:
        qDebug()<<"TEXT";
        lenName = bytes[2];
        qDebug()<<lenName;
        name = bytes.mid(3,lenName);
        msg = bytes.mid(3+lenName,(bytes.length()-(3+lenName)));
        //**************2016.5.22****�޸�****�ж������Ƿ�Ϊһ���ո����ǣ��򵯳���ʾ����������������������������������������
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
    case PRITEXT:
        qDebug()<<"emit";
        if(this->pdlg != NULL)
        {
            this->pdlg->ResolveMsg(bytes);
        }
        emit this->mysignal(bytes);
        break;
    default:
        return -1;
        break;

    }

    return 1;
}
//��������������������������������������ʾ��Ϣ����

/**
 * processPendingDatagram
 * \brief   ����ȴ������ݱ�
 * \return  null
 * \update
    16-5-18: �޸Ĵ�����ΪResolveMsg()
 */

void MultiChatDlg::processPendingDatagram() //
{
    while(socket->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {
       qDebug()<<"222";
       QByteArray datagram; //ӵ�ڴ�Ž��յ����ݱ�
       //��datagram�Ĵ�СΪ�ȴ���������ݱ��Ĵ�С���������ܽ��յ�����������
       datagram.resize(socket->pendingDatagramSize());
       //�������ݱ��������ŵ�datagram��
       socket->readDatagram(datagram.data(),datagram.size());
       //�����ݱ�������ʾ����
       this->ResolveMsg(datagram);

//       ui->receiveMsg->append(datagram);
//       ui->label->setText(datagram);


//       //�ڱ������ӵ�¼״̬
//       numOfOnline++;
//       model->setItem(numOfOnline,0,new QStandardItem("111"));
//       //������ɫ
//       model->item(numOfOnline,0)->setForeground(QBrush(QColor(255,0,0)));
//       //�����ַ�λ��
//       model->item(numOfOnline,0)->setTextAlignment(Qt::AlignCenter);
//       model->setItem(numOfOnline,1,new QStandardItem(QString::fromLocal8Bit("haha")));

//       for(int i = 0; i<3; i++){
//           model->setItem(i,0,new QStandardItem("11111"));
//           //������ɫ
//           model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//           //�����ַ�λ��
//           model->item(i,0)->setTextAlignment(Qt::AlignCenter);
//           model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//       }
    }
}



 bool MultiChatDlg::setHostIP(QString ip)
{
    return this->hostIP->setAddress(ip);
}

//void Widget::newConnect()
//{
//    blockSize = 0; //��ʼ����Ϊ0
//    tcpSocket->abort(); //ȡ�����е�����

////���ӵ�����������ӽ����ȡ������ַ�Ͷ˿ں�
////    tcpSocket->connectToHost(ui->hostLineEdit->text(),
////                             ui->portLineEdit->text().toInt());
//}

//void Widget::displayError(QAbstractSocket::SocketError)
//{
//    qDebug() << tcpSocket->errorString(); //���������Ϣ
//}

//void Widget::readMessage()
//{
//    QString message;
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_8);
//    //�����������汾������Ҫ�ͷ���������ͬ
//    if(blockSize==0) //����Ǹտ�ʼ��������
//    {
//       //�жϽ��յ������Ƿ������ֽڣ�Ҳ�����ļ��Ĵ�С��Ϣ
//       //������򱣴浽blockSize�����У�û���򷵻أ�������������
//       if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//       in >> blockSize;
//    }
//    if(tcpSocket->bytesAvailable() < blockSize) return;
//    //���û�еõ�ȫ�������ݣ��򷵻أ�������������
//    in >> message;
//    //�����յ������ݴ�ŵ�������
//    ui->textBrowser->setText(message);
//    //��ʾ���յ�������
//}


 /**
  * on_sendButton_clicked
  * \brief   ������Ͱ�ť������Ϣ
  * \return  null
  * \update
     16-5-18: �޸Ĵ�����ΪSendMsg()
  */
void MultiChatDlg::on_sendButton_clicked()
{
    QString str =  this->ui->sendMsg->toPlainText();
    str = this->MakeMsg(str,MULTEXT);
    //just point to point
    //this->SendMsg(str,QHostAddress("192.168.0.128"));
    this->SendMsg(str,QHostAddress::Broadcast);
}

/**
 * SendMsg
 * \brief   ������Ϣ����
 * \param   str ��Ҫ���͵��ַ���
 * \param   host ��Ҫ���͵�����
 * \return  ���ط��͵�bytes�������ʧ�ܷ���-1
 * \update
    16-5-18: ����
 */
int MultiChatDlg::SendMsg(QString str,QHostAddress host)
{
    QByteArray data = str.toAscii();
    this->ui->sendMsg->clear();
    return socket->writeDatagram(data,host,PORT);
}


/**
 * on_icon_close_clicked
 * \brief   ����رհ�ť����
 * \return  null
 * \update
    16-5-14: ����
    16-5-23���رշ���������Ϣ
 */
void MultiChatDlg::on_icon_close_clicked()
{
    QString str = this->MakeMsg("",OFFLINE);
    this->SendMsg(str,QHostAddress::Broadcast);
    this->close();
}

/**
 * mousePressEvent
 * \brief   �����갴��׼����ק����
 * \return  null
 * \update
    16-5-14: ����
 */
void MultiChatDlg::mousePressEvent(QMouseEvent *event){
        QPoint windowPos = this->pos();
        QPoint mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;

}

bool MultiChatDlg::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        if(this->isMinimized())
        {
            qDebug()<<"Min";

            ui->icon_close->setFocus();
        }
        if(this->isActiveWindow())
        {
            qDebug()<<"no";
           this->showNormal();
        }
        return true;
    } else {
    // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

/**
 * mouseMoveEvent
 * \brief   �����ק����
 * \return  null
 * \update
    16-5-14: ����
 */
void MultiChatDlg::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}
/**
 * on_icon_mini_clicked
 * \brief   �����С����ť����
 * \return  null
 * \update
    16-5-14: ����
 */

void MultiChatDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}

//����������������������������������������2016.5.22**�����ݲ���ʱ�򣬸ı䷢�Ͱ�ť��״̬Ϊ����***
void MultiChatDlg::on_sendMsg_textChanged()
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
    //ui->sendButton->setStyleSheet("background-color:#FFF");
}




void MultiChatDlg::on_contactList_doubleClicked(const QModelIndex &index)
{
    //�������
    qDebug()<<index.row();
    QString oth = this->model->data(model->index(index.row(),0)).toString();
    qDebug()<<oth;
    //���ip
    QString ip = this->model->data(model->index(index.row(),1)).toString();
    qDebug()<<ip;
    this->pdlg = new PrivateChatDlg();
    this->pdlg->setName(this->name,oth);
    this->pdlg->setHostIP(ip);
    this->pdlg->show();

}
