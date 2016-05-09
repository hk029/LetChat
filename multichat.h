#ifndef MULTICHAT_H
#define MULTICHAT_H

#include <QDialog>
#include <widget.h>

namespace Ui {
class MultiChat;
}

class MultiChat : public QDialog
{
    Q_OBJECT
    
public:
    explicit MultiChat(QWidget *parent = 0);
    ~MultiChat();
    
private:
    Ui::MultiChat *ui;
    Widget *wdg;
};

#endif // MULTICHAT_H
