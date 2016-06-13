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
    //*********************设置event filter*********************//
    this->installEventFilter(this);
    name = "hk";
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
    //1.添加表头
    model = new QStandardItemModel();

    model->setColumnCount(2);

    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("name"));

    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("Ip address"));

    //2.设置表格属性
    ui->contactList->setModel(model);
    //表头信息显示居左
    ui->contactList->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    //设置列宽不可变
    ui->contactList->horizontalHeader()->setResizeMode(0,QHeaderView::Fixed);
    ui->contactList->horizontalHeader()->setResizeMode(1,QHeaderView::Fixed);
    ui->contactList->setColumnWidth(0,150);
    ui->contactList->setColumnWidth(1,100);
    //3.添加行
//    for(int i = 0; i<3; i++){
//        model->setItem(i,0,new QStandardItem("11111"));
//        //设置颜色
//        model->item(i,0)->setForeground(QBrush(QColor(255,0,0)));
//        //设置字符位置
//        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
//        model->setItem(i,1,new QStandardItem(QString::fromLocal8Bit("haha")));
//    }

    QString IP = this->getIP();
    QString s = this->MakeMsg(IP+'/'+this->name,ONLINE);
    this->SendMsg(s,QHostAddress::Broadcast);
//    int row = tItem->row();

    //************************初始内容为空,发送按钮不可用
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
 * \brief   处理等待的数据报
 * \return  null
 * \update
    16-5-13: 重绘透明背景
 */

void MultiChatDlg::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,bkg.scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)); // 绘制不规则背景

}

/**
 * getIP
 * \brief   获取当前主机的ip
 * \return  null
 * \update
    16-5-14: 新增
    16-5-23：修改为能正确获取当前使用的ip
 */

QString MultiChatDlg::getIP()  //获取ip地址
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
 * \brief   构造消息函数
 * \param   str 需要发送的字符串
 * \param   type 构造类型，根据MsgType
 * \return  构造好的消息
 * \update
    16-5-18: 新增
    16-5-23：修改上线消息，解决用户上线互相发送信息的死循环
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
        // |上线信息| ip长度(byte)|ip | name长度(byte) | name|
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
        // |下线信息| ip长度(byte)|ip | name长度(byte) | name|
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
        // |普通消息| name长度 | name| str|
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
 * \brief   解析函数
 * \param   bytes 需要解析的字符串
 * \return  如果没正确解析返回-1
 * \update
    16-5-18: 新增
    16-5-23：修改下线消息，解决删除不在列表中的用户造成的死机

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

    //********************添加变量×××××××××××××××××××
    //QDateTime time;
    //QString reTime;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString reTime = time.toString("hh:mm"); //设置显示格式
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
        //在表格中添加登录状态
        numOfOnline++;
        model->appendRow(new QStandardItem(name));
        rows= model->rowCount();
       // model->setItem(numOfOnline,0,new QStandardItem(ip));
        //设置颜色
        model->item(rows-1,0)->setForeground(QBrush(QColor(255,0,0)));
        //设置字符位置
        model->item(rows-1,0)->setTextAlignment(Qt::AlignCenter);
        model->setItem(rows-1,1,new QStandardItem(ip));
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"] "+reTime+" 上线了...");

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
        tList = model->findItems(ip);//指定的条件
        //update 5-23
        if(!tList.empty()){
            tItem = tList.at(0);//按照第一列的值查找

            row = tItem->row();

            model->removeRow(row);//移除
        }


        //*******************************************************************
        this->ui->receiveMsg->setTextColor("gray");
        this->ui->receiveMsg->append("["+name+"] " + reTime+" 下线了...");
        break;

        case MULTEXT:
        qDebug()<<"TEXT";
        lenName = bytes[2];
        qDebug()<<lenName;
        name = bytes.mid(3,lenName);
        msg = bytes.mid(3+lenName,(bytes.length()-(3+lenName)));
        //**************2016.5.22****修改****判断输入是否为一串空格，若是，则弹出提示××××××××××××××××××××
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
//×××××××××××××××弹出框提示信息美化

/**
 * processPendingDatagram
 * \brief   处理等待的数据报
 * \return  null
 * \update
    16-5-18: 修改处理函数为ResolveMsg()
 */

void MultiChatDlg::processPendingDatagram() //
{
    while(socket->hasPendingDatagrams())  //拥有等待的数据报
    {
       qDebug()<<"222";
       QByteArray datagram; //拥于存放接收的数据报
       //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
       datagram.resize(socket->pendingDatagramSize());
       //接收数据报，将其存放到datagram中
       socket->readDatagram(datagram.data(),datagram.size());
       //将数据报内容显示出来
       this->ResolveMsg(datagram);

//       ui->receiveMsg->append(datagram);
//       ui->label->setText(datagram);


//       //在表格中添加登录状态
//       numOfOnline++;
//       model->setItem(numOfOnline,0,new QStandardItem("111"));
//       //设置颜色
//       model->item(numOfOnline,0)->setForeground(QBrush(QColor(255,0,0)));
//       //设置字符位置
//       model->item(numOfOnline,0)->setTextAlignment(Qt::AlignCenter);
//       model->setItem(numOfOnline,1,new QStandardItem(QString::fromLocal8Bit("haha")));

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


 /**
  * on_sendButton_clicked
  * \brief   点击发送按钮出发消息
  * \return  null
  * \update
     16-5-18: 修改处理函数为SendMsg()
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
 * \brief   发送消息函数
 * \param   str 需要发送的字符串
 * \param   host 需要发送的主机
 * \return  返回发送的bytes数，如果失败返回-1
 * \update
    16-5-18: 新增
 */
int MultiChatDlg::SendMsg(QString str,QHostAddress host)
{
    QByteArray data = str.toAscii();
    this->ui->sendMsg->clear();
    return socket->writeDatagram(data,host,PORT);
}


/**
 * on_icon_close_clicked
 * \brief   如果关闭按钮按下
 * \return  null
 * \update
    16-5-14: 新增
    16-5-23：关闭发送下线信息
 */
void MultiChatDlg::on_icon_close_clicked()
{
    QString str = this->MakeMsg("",OFFLINE);
    this->SendMsg(str,QHostAddress::Broadcast);
    this->close();
}

/**
 * mousePressEvent
 * \brief   如果鼠标按下准备拖拽窗口
 * \return  null
 * \update
    16-5-14: 新增
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
 * \brief   如果拖拽窗口
 * \return  null
 * \update
    16-5-14: 新增
 */
void MultiChatDlg::mouseMoveEvent(QMouseEvent *event){
        this->move(event->globalPos() - this->dPos);
}
/**
 * on_icon_mini_clicked
 * \brief   如果最小化按钮按下
 * \return  null
 * \update
    16-5-14: 新增
 */

void MultiChatDlg::on_icon_mini_clicked()
{
    this->showMinimized();
}

//××××××××××××××××××××2016.5.22**当内容不空时候，改变发送按钮的状态为可用***
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
        ui->sendButton->setStyleSheet("background-color:#666");//灰色
        this->ui->sendButton->setDisabled(bool_dis);
    }
    //ui->sendButton->setStyleSheet("background-color:#FFF");
}




void MultiChatDlg::on_contactList_doubleClicked(const QModelIndex &index)
{
    //获得名字
    qDebug()<<index.row();
    QString oth = this->model->data(model->index(index.row(),0)).toString();
    qDebug()<<oth;
    //获得ip
    QString ip = this->model->data(model->index(index.row(),1)).toString();
    qDebug()<<ip;
    this->pdlg = new PrivateChatDlg();
    this->pdlg->setName(this->name,oth);
    this->pdlg->setHostIP(ip);
    this->pdlg->show();

}
