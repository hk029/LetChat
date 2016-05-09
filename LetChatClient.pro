#-------------------------------------------------
#
# Project created by QtCreator 2016-05-04T22:58:47
#
#-------------------------------------------------

QT       += core gui
QT       += network
TARGET = LetChatClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    multichat.cpp \
    multichatdlg.cpp

HEADERS  += widget.h \
    multichat.h \
    multichatdlg.h

FORMS    += widget.ui \
    multichat.ui \
    multichatdlg.ui
