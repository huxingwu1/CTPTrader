#-------------------------------------------------
#
# Project created by QtCreator 2019-01-05T21:46:12
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CTPTrader01
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ui/baseview.cpp \
    ui/accountinfo.cpp \
    ui/hangqinginfo.cpp \
    ui/weituoinfo.cpp \
    ui/chicanginfo.cpp \
    ui/tradelog.cpp \
    ui/tradeboardinfo.cpp \
    eventengine.cpp \
    dataengine.cpp \
    ctpmdapi.cpp \
    login.cpp \
    ctptdapi.cpp \
    ui/historytradeinfo.cpp \
    mainengine.cpp \
    ctaengine.cpp \
    techindicator.cpp \
    ctastrategybase.cpp \
    strategyatrrsi.cpp \
    myemail.cpp \
    searchinstrument.cpp \
    ui/instrumentinfo.cpp \
    smtp.cpp \
    mydatabase.cpp \
    ui/strategymonitor.cpp \
    login_otheraccount.cpp \
    datafromlocal.cpp

HEADERS  += mainwindow.h \
    ui/baseview.h \
    ui/accountinfo.h \
    ui/hangqinginfo.h \
    ui/weituoinfo.h \
    ui/chicanginfo.h \
    ui/tradelog.h \
    ui/tradeboardinfo.h \
    eventbase.h \
    callback.h \
    cppqueue.h \
    eventengine.h \
    publicstruct.h \
    positionbuffer.h \
    ctpApi/ThostFtdcMdApi.h \
    ctpApi/ThostFtdcTraderApi.h \
    ctpApi/ThostFtdcUserApiDataType.h \
    ctpApi/ThostFtdcUserApiStruct.h \
    dataengine.h \
    ctpmdapi.h \
    login.h \
    ctptdapi.h \
    ctpcommand.h \
    ui/historytradeinfo.h \
    mainengine.h \
    ctaengine.h \
    ctabase.h \
    techindicator.h \
    talib/include/ta_abstract.h \
    talib/include/ta_common.h \
    talib/include/ta_defs.h \
    talib/include/ta_func.h \
    talib/include/ta_libc.h \
    ctastrategybase.h \
    strategyatrrsi.h \
    myemail.h \
    searchinstrument.h \
    ui/instrumentinfo.h \
    smtp.h \
    mydatabase.h \
    ui/strategymonitor.h \
    login_otheraccount.h \
    datafromlocal.h

FORMS    += mainwindow.ui \
    login.ui \
    myemail.ui \
    searchinstrument.ui \
    login_otheraccount.ui \
    datafromlocal.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    ctpApi/thostmduserapi.lib \
    ctpApi/thosttraderapi.lib \
    ctpApi/thostmduserapi.dll \
    ctpApi/thosttraderapi.dll \
    ctpApi/error.xml \
    ctpApi/error.dtd \
    ctpApi/md5.txt \
    icon/stylesheet.qss \
    talib/lib/ta_abstract_cdr.lib \
    talib/lib/ta_common_cdr.lib \
    talib/lib/ta_func_cdr.lib \
    talib/lib/ta_libc_cdr.lib

win32: LIBS += -L$$PWD/ctpApi/ -lthostmduserapi

INCLUDEPATH += $$PWD/ctpApi
DEPENDPATH += $$PWD/ctpApi

win32: LIBS += -L$$PWD/ctpApi/ -lthosttraderapi

INCLUDEPATH += $$PWD/ctpApi
DEPENDPATH += $$PWD/ctpApi

win32: LIBS += -L$$PWD/talib/lib/ -lta_libc_cdr

INCLUDEPATH += $$PWD/talib/include
DEPENDPATH += $$PWD/talib/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/talib/lib/ta_libc_cdr.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/talib/lib/libta_libc_cdr.a
