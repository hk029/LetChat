#ifndef MULTICHATDLG_H
#define MULTICHATDLG_H

#include <QDialog>
#include "allsetting.h"
#include "privatechatdlg.h"

namespace Ui {
class MultiChatDlg;
}

class MultiChatDlg : public QDialog
{
    Q_OBJECT
public:
    explicit MultiChatDlg(QWidget *parent = 0);
    ~MultiChatDlg();
    bool setHostIP(QString);
    QString getIP();
    int setName(QString name);
    QString MakeMsg(QString str,int type);
    int ResolveMsg(QByteArray bytes);
    int SendMsg(QString str,QHostAddress host);
    int onLine();
    QUdpSocket *socket;
signals:
    void mysignal(QByteArray bytes);
private slots:
    void processPendingDatagram();
    void pridlgclose();
    void on_icon_close_clicked();

    void on_icon_mini_clicked();
    void on_sendButton_clicked();
    void on_sendMsg_textChanged();

    void on_contactList_doubleClicked(const QModelIndex &index);

protected:
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);
     bool eventFilter(QObject *, QEvent *);
     void mouseMoveEvent(QMouseEvent *event);
//    void newConnect(); //连接服务器
//    void readMessage();  //接收数据
//void displayError(QAbstractSocket::SocketError);  //显示错误






private:
    //SingleChatDlg *chatdlg;
    PrivateChatDlg *pdlg;
    int blockSize;
    QHostAddress *hostIP;
    int port;
    QString name;
    Ui::MultiChatDlg *ui;


    QStandardItemModel  *model;
    int numOfOnline;


    QPoint dPos;
    QPixmap bkg;


//    QTcpSocket *tcpSocket;
};

#endif // MULTICHATDLG_H



