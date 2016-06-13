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
    privatechatdlg.cpp

HEADERS  += \
    multichatdlg.h \
    maindlg.h \
    allsetting.h \
    privatechatdlg.h

FORMS    += \
    multichatdlg.ui \
    maindlg.ui \
    privatechatdlg.ui

RESOURCES += \
    image.qrc

OTHER_FILES += \
    style/base.qss
