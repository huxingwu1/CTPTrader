#include "mainwindow.h"
#include <QApplication>
#include <string.h>
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QFile>
#include <QString>
#include <QMetaType>
#include <QVector>
#include "techindicator.h"


// 全局变量
MainEngine* me = nullptr;
MainWindow* w = nullptr;

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(me)
    {
        me->outputMessage(type, context, msg);
    }
}

int main(int argc, char *argv[])
{
    // 初始化talib
    TechIndicator::initialize();

    // 开启主进程
    QApplication a(argc, argv);
    me = new MainEngine();
    qInstallMessageHandler(outputMessage);     //注册日志MessageHandler
    qRegisterMetaType<QVector<int>>("QVector<int>");    //消除警告  QObject::connect: Cannot queue arguments of type 'QVector<int>'

    //打开qss失败
//    QFile qss("icon/stylesheet.qss");
//    qss.open(QFile::ReadOnly);
//    QTextStream filetext(&qss);
//    QString stylesheet = filetext.readAll();
//    a.setStyleSheet(stylesheet);
//    qss.close();

    w = new MainWindow();
    //w.setWindowFlags(w.windowFlags()&~Qt::WindowMinMaxButtonsHint);   //隐藏最大化最小化按钮
    //w.setWindowFlags(w.windowFlags()&~Qt::WindowMinMaxButtonsHint|Qt::WindowMinimizeButtonHint);
    w->show();
    a.exec();

    if (me)
    {
        delete me;
        me = nullptr;
    }
    // 关闭talib
    TechIndicator::taShutdown();
    return 0;
}
