#ifndef MYEMAIL_H
#define MYEMAIL_H

#include <QWidget>
#include <QString>
#include <QSettings>    //写配置文件
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include "smtp.h"

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

private:
    Ui::MyEmail *ui;
    QSettings *configIniRead;    //读配置文件
    QSettings *configIniWrite;   //写配置文件

    QString server;
    QString sendAddress;
    QString password;
    QString receviceAddress;
public:
    Smtp *sendmail {nullptr};    //邮箱指针
    void sendEmail(QString theme, QString content);

};

#endif // MYEMAIL_H
