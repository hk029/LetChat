#ifndef MULTICHATDLG_H
#define MULTICHATDLG_H

#include <QDialog>

namespace Ui {
class MultiChatDlg;
}

class MultiChatDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit MultiChatDlg(QWidget *parent = 0);
    ~MultiChatDlg();
    
private:
    Ui::MultiChatDlg *ui;
};

#endif // MULTICHATDLG_H
