#ifndef ALLSETTING_H
#define ALLSETTING_H

#include "QDebug"
#include <QMouseEvent>
#include <QStandardItemModel>
#include "QPainter"
#include <QMessageBox>
#include <QtNetwork>
const int PORT=11223;

enum MsgType{
    ONLINE=1,
    OFFLINE,
    MULTEXT,
    PRITEXT

};

#endif // ALLSETTING_H