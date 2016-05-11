#ifndef MAINDLG_H
#define MAINDLG_H

#include <QDialog>

#include <QtNetwork>
#include "QPainter"
#include <multichatdlg.h>
#include <QMouseEvent>
#define PORT 11223

namespace Ui {
class MainDlg;
}

class MainDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit MainDlg(QWidget *parent = 0);
    ~MainDlg();
protected:
     void paintEvent(QPaintEvent *);
     void mousePressEvent(QMouseEvent *);
     void mouseMoveEvent(QMouseEvent *);

private slots:
     void on_icon_close_clicked();

     void on_IntoChatRoom_clicked();

     void on_icon_mini_clicked();

private:
    Ui::MainDlg *ui;
    QPoint dPos;
    QPixmap bkg;
    MultiChatDlg *multidlg;
};

#endif // MAINDLG_H
