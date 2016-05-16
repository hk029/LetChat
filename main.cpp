#include <QtGui/QApplication>


#include "multichatdlg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    MultiChatDlg m;
    m.show();
//    Widget w;
//    w.show();
    
    return a.exec();
}
