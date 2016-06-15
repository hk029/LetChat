#ifndef PRIVATECHATDLG_H
#define PRIVATECHATDLG_H

#include <QDialog>
#include "allsetting.h"

namespace Ui {
class PrivateChatDlg;
}

class PrivateChatDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrivateChatDlg(QWidget *parent = 0);
    PrivateChatDlg(QUdpSocket *s);
    ~PrivateChatDlg();
    void setSocket(QUdpSocket *s);
    void setName(QString n,QString oth);
    void refreshtext();
    QString getName();
    bool setHostIP(QString);
    QString MakeMsg(QString str,int type);
    int ResolveMsg(QByteArray bytes);
    int SendMsg(QString str,QHostAddress host);
   QUdpSocket *socket;
signals:
    void closedlg();
protected:
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void paintEvent(QPaintEvent *event);
private slots:
     void on_icon_close_clicked();
    void processPendingDatagram();

     void on_icon_mini_clicked();

     void on_sendMsg_textChanged();

     void on_sendButton_clicked();

private:
    Ui::PrivateChatDlg *ui;
    QString name;
    QString OtherName;
    QHostAddress *hostIP;
    QPoint dPos;
    QPixmap bkg;

};

#endif // PRIVATECHATDLG_H
