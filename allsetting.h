#ifndef ALLSETTING_H
#define ALLSETTING_H

#include "QDebug"
#include <QMouseEvent>
#include <QStandardItemModel>
#include "QPainter"
#include <QtNetwork>
#include <multichatdlg.h>
const int PORT=11223;

enum MsgType{
    ONLINE=1,
    OFFLINE,
    TEXT
};

#endif // ALLSETTING_H
