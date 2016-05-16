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
    numOfOnline = 0;

    //new added
    //1.添加表头
    model = new QStandardItemModel();

    model->setColumnCount(2);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("Ip address"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("name"));
    //2.设置表格属性
    ui->contactList->setModel(model);
    //表头信息显示居左
    ui->contactList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //设置列宽不可变
    ui->contactList->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    ui->contactList->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->contactList->setColumnWidth(0,101);
    ui->contactList->setColumnWidth(1,102);
    //3.添加行
//    for(int i = 0; i<3; i++){
//        model->setItem(i,0,new QStandardItem("11111"));
//        //设置颜色
//        model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//        //设置字符位置
//        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
//        model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//    }


    //4.删除制定行
//    QList<QStandardItem *> tList = model->findItems ("11111");//指定的条件

//    QStandardItem* tItem = tList.at(0);//按照第一列的值查找

//    int row = tItem->row();

//    model->removeRow(row);//移除


}

MultiChatDlg::~MultiChatDlg()
{
    delete ui;
}


void MultiChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // 绘制不规则背景

}

QString MultiChatDlg::getIP()  //获取ip地址
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

void MultiChatDlg::processPendingDatagram() //处理等待的数据报
{
    while(socket->hasPendingDatagrams())  //拥有等待的数据报
    {

       QByteArray datagram; //拥于存放接收的数据报
       //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
       datagram.resize(socket->pendingDatagramSize());
       //接收数据报，将其存放到datagram中
       socket->readDatagram(datagram.data(),datagram.size());
       //将数据报内容显示出来
       ui->receiveMsg->append(datagram);
//       ui->label->setText(datagram);


       //在表格中添加登录状态
       numOfOnline++;
       model->setItem(numOfOnline,0,new QStandardItem("111"));
       //设置颜色
       model->item(numOfOnline,0)->setForeground(QBrush(QColor(255,0,0)));
       //设置字符位置
       model->item(numOfOnline,0)->setTextAlignment(Qt::AlignCenter);
       model->setItem(numOfOnline,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//       for(int i = 0; i<3; i++){
//           model->setItem(i,0,new QStandardItem("11111"));
//           //设置颜色
//           model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//           //设置字符位置
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

void MultiChatDlg::on_sendButton_clicked()
{
    QString str =  this->ui->sendMsg->toPlainText();
    str = name+":"+str;
    QByteArray data = str.toAscii();
    this->ui->receiveMsg->append(str);
    socket->writeDatagram(data,*hostIP,PORT);
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
