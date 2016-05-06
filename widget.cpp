#include "widget.h"
#include "ui_widget.h"
#include <QtNetwork>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    qDebug() <<"IP Address: "<<info.addresses();
    qDebug() <<"localHostName: "<<localHostName;

    socket = new QUdpSocket(this);
    socket->bind(11223,QUdpSocket::ShareAddress);
    connect(socket,SIGNAL(readyRead()),this,SLOT(processPendingDatagram()));

//    tcpSocket = new QTcpSocket(this);
//    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
//    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
//             this,SLOT(displayError(QAbstractSocket::SocketError)));
}

Widget::~Widget()
{
    delete ui;
}



void Widget::processPendingDatagram() //�����ȴ������ݱ�
{
    while(socket->hasPendingDatagrams())  //ӵ�еȴ������ݱ�
    {

       QByteArray datagram; //ӵ�ڴ�Ž��յ����ݱ�
//��datagram�Ĵ�СΪ�ȴ����������ݱ��Ĵ�С���������ܽ��յ�����������
       datagram.resize(socket->pendingDatagramSize());
       //�������ݱ��������ŵ�datagram��
       socket->readDatagram(datagram.data(),datagram.size());
       //�����ݱ�������ʾ����
       ui->textBrowser->setText(datagram);
//       ui->label->setText(datagram);
    }
}

void Widget::on_pushButton_clicked()
{
    QString str =  this->ui->plainTextEdit->toPlainText();
    QByteArray data = str.toAscii();
    socket->writeDatagram(data,QHostAddress("192.168.0.128"),11223);
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