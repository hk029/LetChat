#include "multichatdlg.h"
#include "ui_multichatdlg.h"
//#include <QStandardItemModel>
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

//    tcpSocket = new QTcpSocket(this);
//    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
//    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
    numOfOnline = 0;

    //new added
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
    ui->contactList->setColumnWidth(0,101);
    ui->contactList->setColumnWidth(1,102);
    //3.�����
//    for(int i = 0; i<3; i++){
//        model->setItem(i,0,new QStandardItem("11111"));
//        //������ɫ
//        model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//        //�����ַ�λ��
//        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
//        model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//    }


    //4.ɾ���ƶ���
//    QList<QStandardItem *> tList = model->findItems ("11111");//ָ��������

//    QStandardItem* tItem = tList.at(0);//���յ�һ�е�ֵ����

//    int row = tItem->row();

//    model->removeRow(row);//�Ƴ�


}

MultiChatDlg::~MultiChatDlg()
{
    delete ui;
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
       ui->receiveMsg->append(datagram);
//       ui->label->setText(datagram);


       //�ڱ������ӵ�¼״̬
       numOfOnline++;
       model->setItem(numOfOnline,0,new QStandardItem("111"));
       //������ɫ
       model->item(numOfOnline,0)->setForeground(QBrush(QColor(255,0,0)));
       //�����ַ�λ��
       model->item(numOfOnline,0)->setTextAlignment(Qt::AlignCenter);
       model->setItem(numOfOnline,1,new QStandardItem(QString::fromLocal8Bit("haha")));
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
    str = name+":"+str;
    QByteArray data = str.toAscii();
    socket->writeDatagram(data,*hostIP,PORT);
    this->ui->sendMsg->clear();
}


