#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

#define PORT 11223

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    
public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    bool setHostIP(QString);

private slots:
    void processPendingDatagram();
//    void newConnect(); //���ӷ�����
//    void readMessage();  //��������
//void displayError(QAbstractSocket::SocketError);  //��ʾ����

void on_pushButton_clicked();

private:
    int blockSize;
    QHostAddress *hostIP;
    int port;
    Ui::Widget *ui;
    QUdpSocket *socket;
//    QTcpSocket *tcpSocket;
};

#endif // WIDGET_H
