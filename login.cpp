#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //初始化登录界面信息
    Init_Login_info();
    setStyleSheet("font:11pt ;background-color: rgb( 90, 90, 90);color: white;");    //字体大小颜色
}

Login::~Login()
{
    delete ui;
}

//   初始化登录界面信息
void Login::Init_Login_info()
{
//    ui->Md_Edit->setText("tcp://180.168.146.187:10010");
//    ui->Td_Edit->setText("tcp://180.168.146.187:10000");
//    ui->Broker_Edit->setText("9999");
//    ui->Account_Edit->setText("121995");                       //huxw的账号
//    ui->Password_Edit->setEchoMode(QLineEdit::Password);
//    ui->Password_Edit->setText("Hxw19930922");

      //读文件形式
//    QList<QString> list;
//    QFile file("Account.txt");
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        //循环读下去
//        while (!file.atEnd())
//        {
//            QByteArray line = file.readLine();
//            QString str(line);
//            list.append(str);
//        }

//        //默认选中保存登录信息按钮
//        ui->Md_Edit->setText(list[0]);    //list[0].split(",").at(1)
//        ui->Td_Edit->setText(list[1]);
//        ui->Broker_Edit->setText(list[2]);
//        ui->Account_Edit->setText(list[3]);
//        ui->Password_Edit->setEchoMode(QLineEdit::Password);
//        ui->SaveAccount->setChecked(true);
//    }

    //读配置文件
    configIniRead = new QSettings("Account/Account.ini", QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    QString md = configIniRead->value("/account/mdAddress").toString();
    ui->Md_Edit->setText(md);
    QString td = configIniRead->value("account/tdAddress").toString();
    ui->Td_Edit->setText(td);
    QString br = configIniRead->value("account/broker").toString();
    ui->Broker_Edit->setText(br);
    QString ac = configIniRead->value("account/account").toString();
    ui->Account_Edit->setText(ac);
    QString ps = configIniRead->value("account/password").toString();
    if(0!=ps.size())
        ui->Password_Edit->setText(ps);
    //密码格式，保存账户
    ui->Password_Edit->setEchoMode(QLineEdit::Password);
    ui->SaveAccount->setChecked(true);

    qDebug()<<QString::fromLocal8Bit("Login初始化成功");

}


//  点击ok登录,保存账户信息，登录，初始化MD和TD，连接成功后关闭Dialog窗口
void Login::on_buttonBox_accepted()
{
//    //将信息保存
//    if(ui->SaveAccount->isChecked())
//    {
//        QFile file("Account.txt");
//        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
//            return;
//         QTextStream out(&file);
//         out << ui->Md_Edit->text() << "\n";
//         out << ui->Td_Edit->text() << "\n";
//         out << ui->Broker_Edit->text() << "\n";
//         out << ui->Account_Edit->text() << "\n";
//         file.close();
//    }

//     //调用mainwindow中的MdTdLogin登录函数
//     QString md = ui->Md_Edit->text();
//     QString td = ui->Td_Edit->text();
//     QString brokerid = ui->Broker_Edit->text();
//     QString account = ui->Account_Edit->text();
//     QString password = ui->Password_Edit->text();
//     QString ACCOUNT = md + "," + td + "," +brokerid +","+account +","+password;
//     //emit sendAccount(ACCOUNT);

    //创建Account 目录
    QString path = "Account/";
    QDir temp;
    if (!temp.exists(path))
    {
        bool n = temp.mkpath(path);   //Creates the directory path dirPath.
    }

    //将信息保存
    if(ui->SaveAccount->isChecked())
    {
        //写配置文件
        //QSettings构造函数的第一个参数是ini文件的路径,第二个参数表示针对ini文件,第三个参数可以缺省
        configIniWrite = new QSettings("Account/Account.ini", QSettings::IniFormat);
        //向ini文件中写入内容,setValue函数的两个参数是键值对
        //向ini文件的第一个节写入内容,account节下的第一个参数
        configIniWrite->setValue("/account/mdAddress", ui->Md_Edit->text());
        //向ini文件的第一个节写入内容,ip节下的第二个参数
        configIniWrite->setValue("account/tdAddress", ui->Td_Edit->text());
        configIniWrite->setValue("account/broker", ui->Broker_Edit->text());
        configIniWrite->setValue("account/account", ui->Account_Edit->text());

        if(ui->SavePassword->isChecked())
            configIniWrite->setValue("account/password", ui->Password_Edit->text());
        else
            configIniWrite->setValue("account/password", "");
    }

    //调用mainwindow中的MdTdLogin登录函数
    QString md = ui->Md_Edit->text();
    QString td = ui->Td_Edit->text();
    QString brokerid = ui->Broker_Edit->text();
    QString account = ui->Account_Edit->text();
    QString password = ui->Password_Edit->text();
    QString ACCOUNT = md + "," + td + "," +brokerid +","+account +","+password;

    //如果密码为空
    if(0==md.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录错误"),QString::fromLocal8Bit("行情地址不能为空"),QMessageBox::Ok | QMessageBox::Cancel);
    }
    else if(0==td.size())
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
        qDebug()<<QString::fromLocal8Bit("发送账户成功");
        //调用mainwindow中的MDLogin登录函数
        emit sendAccount(ACCOUNT);
    }

}

//  点击取消按钮后，再次点击登录框不弹出
void Login::on_buttonBox_rejected()
{

}
