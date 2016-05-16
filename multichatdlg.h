#ifndef MULTICHATDLG_H
#define MULTICHATDLG_H

#include <QDialog>

#include <QtNetwork>
#include <QStandardItemModel>

#include "QPainter"
enum MsgType{
    ONLINE,
    OFFLINE,
    TEXT
};

#define PORT 11223
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
    QString MakeMsg(QString str,int type);
private slots:
    void processPendingDatagram();

    void on_icon_close_clicked();

    void on_icon_mini_clicked();

protected:
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);

     void mouseMoveEvent(QMouseEvent *event);
//    void newConnect(); //���ӷ�����
//    void readMessage();  //��������
//void displayError(QAbstractSocket::SocketError);  //��ʾ����


void on_sendButton_clicked();

void on_loginButton_clicked();

private:
    int blockSize;
    QHostAddress *hostIP;
    int port;
    QString name;
    Ui::MultiChatDlg *ui;
    QUdpSocket *socket;


    QStandardItemModel  *model;
    int numOfOnline;


    QPoint dPos;
    QPixmap bkg;


//    QTcpSocket *tcpSocket;
};

#endif // MULTICHATDLG_H
