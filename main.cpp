#include <QtGui/QApplication>

#include "multichatdlg.h"
#include "maindlg.h"
#include "QFontDatabase"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
//    QFontDatabase database;
//    foreach (const QString &family, database.families())
//    {
//         qDebug()<<family;
//    }
//    foreach (const QString &family, database.families(QFontDatabase::SimplifiedChinese))
//    {
//        qDebug()<<family;
//    }
    //获取所有网络接口的列表
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list) //遍历每一个网络接口
    {
        qDebug() << "Device: "<<interface.name(); //设备名
    //硬件地址
    qDebug() << "HardwareAddress: "<<interface.hardwareAddress();

    //获取IP地址条目列表，每个条目中包含一个IP地址，
    //一个子网掩码和一个广播地址
       QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)//遍历每个IP地址条目
        {
           qDebug()<<"IP Address: "<<entry.ip().toString(); //IP地址
           qDebug()<<"Netmask: "<<entry.netmask().toString(); //子网掩码
         qDebug()<<"Broadcast: "<<entry.broadcast().toString();//广播地址
        }
    }
    MultiChatDlg m;
    m.show();
//    Widget w;
//    w.show();
    
    return a.exec();
}
