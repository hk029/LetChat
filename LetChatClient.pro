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
    multichatdlg.cpp \
    maindlg.cpp \
    privatechatdlg.cpp \
    logindlg.cpp

HEADERS  += \
    multichatdlg.h \
    maindlg.h \
    allsetting.h \
    privatechatdlg.h \
    logindlg.h

FORMS    += \
    multichatdlg.ui \
    maindlg.ui \
    privatechatdlg.ui \
    logindlg.ui

RESOURCES += \
    image.qrc

OTHER_FILES += \
    style/base.qss
