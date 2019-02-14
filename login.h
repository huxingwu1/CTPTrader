#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <QString>
#include <QSettings>    //写配置文件
#include <QDebug>
#include <QMessageBox>
#include <QDir>

/*
 * 1.用xml或者配置文件保存信息
 * 2.登陆框为空时弹框提示
 * 3.保存密码，哈希加密
 */

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

signals:
    void sendAccount(QString);

private slots:
    //  点击ok登录
    void on_buttonBox_accepted();

    //  点击cancel取消登录
    void on_buttonBox_rejected();


private:
    //初始化登录界面信息
    void Init_Login_info();

private:
    Ui::Login *ui;

    QSettings *configIniRead;    //读配置文件
    QSettings *configIniWrite;   //写配置文件
};

#endif // LOGIN_H
