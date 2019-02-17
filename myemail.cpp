#include "myemail.h"
#include "ui_myemail.h"

MyEmail::MyEmail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyEmail)
{
    ui->setupUi(this);
    setStyleSheet("font:11pt ;background-color: rgb( 90, 90, 90);color: white;");    //字体大小颜色
    setWindowTitle(QString::fromLocal8Bit("添加邮件"));
    setWindowIcon(QIcon(":/icon/email.png"));
    //setGeometry(x,y,w,h);
    setMinimumSize(300,400);
    setMaximumSize(300,400);

    //读配置文件
    configIniRead = new QSettings("Account/Email.ini", QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString server = configIniRead->value("email/server").toString();
    ui->server->setText(server);
    QString send = configIniRead->value("email/sendAddress").toString();
    ui->sendAddress->setText(send);
    QString password = configIniRead->value("email/password").toString();
    ui->password->setText(password);
    QString recevice = configIniRead->value("email/recevice").toString();
    ui->receviceAddress->setText(recevice);
    ui->radioButton_save->setChecked(true);
}

MyEmail::~MyEmail()
{
    delete ui;
}

// 确认按钮槽函数
void MyEmail::on_pushButton_clicked()
{
    //创建Account 目录
    QString path = "Account/";
    QDir temp;
    if (!temp.exists(path))
    {
        bool n = temp.mkpath(path);   //Creates the directory path dirPath.
    }
    server = ui->server->text();
    sendAddress = ui->sendAddress->text();
    password = ui->password->text();
    receviceAddress =  ui->receviceAddress->text();
    //将信息保存
    if(ui->radioButton_save->isChecked())
    {
        //写配置文件
        configIniWrite = new QSettings("Account/Email.ini", QSettings::IniFormat);
        configIniWrite->setValue("email/server", server);
        configIniWrite->setValue("email/sendAddress", sendAddress);
        configIniWrite->setValue("email/password", password);
        configIniWrite->setValue("email/recevice", receviceAddress);
    }
    QString EMAIL = server + "," + sendAddress + "," +password +","+receviceAddress;
    sendEmail(EMAIL);
}
