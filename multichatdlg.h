#ifndef MULTICHATDLG_H
#define MULTICHATDLG_H

#include <QDialog>

#include <QtNetwork>
#include "QPainter"

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

private slots:
    void processPendingDatagram();

    void on_icon_close_clicked();

    void on_icon_mini_clicked();

protected:
     void paintEvent(QPaintEvent *event);
     void mousePressEvent(QMouseEvent *event);

     void mouseMoveEvent(QMouseEvent *event);
//    void newConnect(); //连接服务器
//    void readMessage();  //接收数据
//void displayError(QAbstractSocket::SocketError);  //显示错误


void on_sendButton_clicked();

private:
    int blockSize;
    QHostAddress *hostIP;
    int port;
    QString name;
    Ui::MultiChatDlg *ui;
    QUdpSocket *socket;
    QPoint dPos;
    QPixmap bkg;
//    QTcpSocket *tcpSocket;
};

#endif // MULTICHATDLG_H
