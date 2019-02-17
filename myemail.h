#ifndef MYEMAIL_H
#define MYEMAIL_H

#include <QWidget>
#include <QString>
#include <QSettings>    //写配置文件
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "smtp.h"

/*
 * 获得邮箱信息，通过信号与槽传给mainengine中的结构体.
 */

namespace Ui {
class MyEmail;
}

class MyEmail : public QWidget
{
    Q_OBJECT

public:
    explicit MyEmail(QWidget *parent = 0);
    ~MyEmail();

private slots:
    void on_pushButton_clicked();

signals:
    void sendEmail(QString);

private:
    Ui::MyEmail *ui;
    QSettings *configIniRead;    //读配置文件
    QSettings *configIniWrite;   //写配置文件

    QString server;
    QString sendAddress;
    QString password;
    QString receviceAddress;
};

#endif // MYEMAIL_H
