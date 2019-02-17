#include "login_otheraccount.h"
#include "ui_login_otheraccount.h"

Login_otheraccount::Login_otheraccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login_otheraccount)
{
    ui->setupUi(this);
    setStyleSheet("font:11pt ;background-color: rgb( 90, 90, 90);color: white;");    //字体大小颜色
}

Login_otheraccount::~Login_otheraccount()
{
    delete ui;
}

// 点击 添加账号密码
void Login_otheraccount::on_pushButton_clicked()
{
    QString td = ui->td->text();
    QString brokerid = ui->brokerid->text();
    QString account = ui->account->text();
    QString password = ui->password->text();
    QString ACCOUNT = td + "," +brokerid +","+account +","+password;

    //如果密码为空
    if(0==td.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录错误"),QString::fromLocal8Bit("交易地址不能为空"),QMessageBox::Ok | QMessageBox::Cancel);
    }
    else if(0==brokerid.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录错误"),QString::fromLocal8Bit("brokerid不能为空"),QMessageBox::Ok | QMessageBox::Cancel);
    }
    else if(0==account.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录错误"),QString::fromLocal8Bit("账号不能为空"),QMessageBox::Ok | QMessageBox::Cancel);
    }
    else if(0==password.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录错误"),QString::fromLocal8Bit("密码不能为空"),QMessageBox::Ok | QMessageBox::Cancel);
    }
    else
    {
        //保存日志
        qDebug()<<ACCOUNT ;
        qDebug()<<QString::fromLocal8Bit("添加多账户 发送成功");
        //调用mainwindow中的MDLogin登录函数
        emit sendAccount(ACCOUNT);
    }
}
