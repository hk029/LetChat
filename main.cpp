#include <QtGui/QApplication>

#include "multichatdlg.h"
#include "maindlg.h"
#include "QFontDatabase"
#include "logindlg.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");

    QTextCodec::setCodecForTr(codec);

    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

    LoginDlg login;
    MultiChatDlg m;
    login.exec();
    QString name;
    name = login.getName();
    if(name == "")
        return a.exec();
    else
    {
       m.setName(name);
       m.onLine();
       m.show();
       return a.exec();
    }

       return a.exec();
}
