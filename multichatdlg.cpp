#include "multichatdlg.h"
#include "ui_multichatdlg.h"
//#include <QStandardItemModel>
#include <QMessageBox>
MultiChatDlg::MultiChatDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultiChatDlg)
{
    ui->setupUi(this);

    name = "my";
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
                        "background-color:#3dce3d;color:white;"
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

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Ip address"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("name"));
    //2.���ñ������
    ui->contactList->setModel(model);
    //��ͷ��Ϣ��ʾ����
    ui->contactList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //�����п��ɱ�
    ui->contactList->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    ui->contactList->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->contactList->setColumnWidth(0,68);
    ui->contactList->setColumnWidth(1,66);
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

}

MultiChatDlg::~MultiChatDlg()
{
    delete ui;
}


void MultiChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // ���Ʋ����򱳾�

}

QString MultiChatDlg::getIP()  //��ȡip��ַ
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       //����ʹ��IPv4��ַ
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
           return address.toString();
    }
    return 0;
}
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
        case TEXT:
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
    int row;
    //********************

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
        model->setItem(numOfOnline,0,new QStandardItem(ip));
        //������ɫ
        model->item(numOfOnline,0)->setForeground(QBrush(QColor(255,0,0)));
        //�����ַ�λ��
        model->item(numOfOnline,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(numOfOnline,1,new QStandardItem(name));
        //***************************************************
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"]"+" ������...");
        break;

        case OFFLINE:
        //************************chengcheng*********************************
        len = bytes[2];
        ip = bytes.mid(3,len);
        nameStartIndex = 2+len+1+1;
        len = bytes[2+len+1];

        name = bytes.mid(nameStartIndex,len);
        tList = model->findItems(ip);//ָ��������

        tItem = tList.at(0);//���յ�һ�е�ֵ����

        row = tItem->row();

        model->removeRow(row);//�Ƴ�
        //*******************************************************************
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"]"+" ������...");
        break;

        case TEXT:
        qDebug()<<"TEXT";
        lenName = bytes[2];
        qDebug()<<lenName;
        name = bytes.mid(3,lenName);
        msg = bytes.mid(3+lenName,(bytes.length()-(3+lenName)));
        //**********************�ж�����Ϊ�գ��򵯳���ʾ����������������������������������������
        if(msg.isEmpty()){
            QMessageBox::warning(this, "����", QString::fromLocal8Bit("���͵����ݲ���Ϊ��"));
            break;
        }
        if(name == this->name){
            this->ui->receiveMsg->setTextColor("red");
        }
        else{
            this->ui->receiveMsg->setTextColor("blue");
        }
        this->ui->receiveMsg->append(name+":\n"+msg);
        break;
    default:
        qDebug()<<"de";
        break;

    }

    return 1;
}


void MultiChatDlg::processPendingDatagram() //����ȴ������ݱ�
{
    while(socket->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {

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

void MultiChatDlg::on_sendButton_clicked()
{

    QString str =  this->ui->sendMsg->toPlainText();
    str = this->MakeMsg(str,TEXT);
    this->SendMsg(str,QHostAddress::Broadcast);
}

int MultiChatDlg::SendMsg(QString str,QHostAddress host)
{
    QByteArray data = str.toAscii();
    socket->writeDatagram(data,host,PORT);
    this->ui->sendMsg->clear();
}

void MultiChatDlg::on_icon_close_clicked()
{
    this->close();
}


void MultiChatDlg::mousePressEvent(QMouseEvent *event){
        QPoint windowPos = this->pos();
        QPoint mousePos = event->globalPos();
        this->dPos = mousePos - windowPos;

}
void MultiChatDlg::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}

void MultiChatDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}
