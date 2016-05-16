#include <QtGui/QApplication>

#include "multichatdlg.h"
#include "maindlg.h"
#include "QFontDatabase"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
//    QFontDatabase database;
//    foreach (const QString &family, database.families())
//    {
//         qDebug()<<family;
//    }
//    foreach (const QString &family, database.families(QFontDatabase::SimplifiedChinese))
//    {
//        qDebug()<<family;
//    }
    MultiChatDlg m;
    m.show();
//    Widget w;
//    w.show();
    
    return a.exec();
}
