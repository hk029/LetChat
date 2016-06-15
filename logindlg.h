#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>
#include <multichatdlg.h>
#include "allsetting.h"
namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginDlg(QWidget *parent = 0);
    ~LoginDlg();
    QString getName();
private slots:
    void on_icon_mini_clicked();

    void on_icon_close_clicked();

    void on_pushButton_clicked();

protected:
     void paintEvent(QPaintEvent *);
     void mousePressEvent(QMouseEvent *);
     void mouseMoveEvent(QMouseEvent *);

private:
    Ui::LoginDlg *ui;
    QString name;
    MultiChatDlg *dlg;

    QPoint dPos;
    QPixmap bkg;
};

#endif // LOGINDLG_H
