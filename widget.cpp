#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);


    hostIP = new QHostAddress(QHostAddress::Broadcast);
    socket = new QUdpSocket(this);
    socket->bind(PORT,QUdpSocket::ShareAddress);
    connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));

//    tcpSocket = new QTcpSocket(this);
//    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
//    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),

}

Widget::~Widget()
{
    delete ui;
}

QString Widget::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       //我们使用IPv4地址
       if(address.protocol() == QAbstractSocket::IPv4Protocol)
           return address.toString();
    }
    return 0;
}

void Widget::processPendingDatagram() //处理等待的数据报
{
    while(socket->hasPendingDatagrams())  //拥有等待的数据报
    {

       QByteArray datagram; //拥于存放接收的数据报
//让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
       datagram.resize(socket->pendingDatagramSize());
       //接收数据报，将其存放到datagram中
       socket->readDatagram(datagram.data(),datagram.size());
       //将数据报内容显示出来
       ui->textBrowser->append(datagram);
//       ui->label->setText(datagram);
    }
}

void Widget::on_pushButton_clicked()
{
    QString str =  this->ui->plainTextEdit->toPlainText();
    str = this->getIP()+":"+str;
    QByteArray data = str.toAscii();
    socket->writeDatagram(data,*hostIP,PORT);
    this->ui->plainTextEdit->clear();
}


 bool Widget::setHostIP(QString ip)
{
    return this->hostIP->setAddress(ip);
}

//void Widget::newConnect()
//{
//    blockSize = 0; //初始化其为0
//    tcpSocket->abort(); //取消已有的连接

////连接到主机，这里从界面获取主机地址和端口号
////    tcpSocket->connectToHost(ui->hostLineEdit->text(),
////                             ui->portLineEdit->text().toInt());
//}

//void Widget::displayError(QAbstractSocket::SocketError)
//{
//    qDebug() << tcpSocket->errorString(); //输出错误信息
//}

//void Widget::readMessage()
//{
//    QString message;
//    QDataStream in(tcpSocket);
//    in.setVersion(QDataStream::Qt_4_8);
//    //设置数据流版本，这里要和服务器端相同
//    if(blockSize==0) //如果是刚开始接收数据
//    {
//       //判断接收的数据是否有两字节，也就是文件的大小信息
//       //如果有则保存到blockSize变量中，没有则返回，继续接收数据
//       if(tcpSocket->bytesAvailable() < (int)sizeof(quint16)) return;
//       in >> blockSize;
//    }
//    if(tcpSocket->bytesAvailable() < blockSize) return;
//    //如果没有得到全部的数据，则返回，继续接收数据
//    in >> message;
//    //将接收到的数据存放到变量中
//    ui->textBrowser->setText(message);
//    //显示接收到的数据
//}
