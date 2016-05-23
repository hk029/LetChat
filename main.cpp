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
    //��ȡ��������ӿڵ��б�
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list) //����ÿһ������ӿ�
    {
        qDebug() << "Device: "<<interface.name(); //�豸��
    //Ӳ����ַ
    qDebug() << "HardwareAddress: "<<interface.hardwareAddress();

    //��ȡIP��ַ��Ŀ�б�ÿ����Ŀ�а���һ��IP��ַ��
    //һ�����������һ���㲥��ַ
       QList<QNetworkAddressEntry> entryList= interface.addressEntries();
        foreach(QNetworkAddressEntry entry,entryList)//����ÿ��IP��ַ��Ŀ
        {
           qDebug()<<"IP Address: "<<entry.ip().toString(); //IP��ַ
           qDebug()<<"Netmask: "<<entry.netmask().toString(); //��������
         qDebug()<<"Broadcast: "<<entry.broadcast().toString();//�㲥��ַ
        }
    }
    MultiChatDlg m;
    m.show();
//    Widget w;
//    w.show();
    
    return a.exec();
}
