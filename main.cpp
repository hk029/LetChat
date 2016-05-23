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

    MultiChatDlg m;
    m.show();
//    Widget w;
//    w.show();
    
    return a.exec();
}
