#include "datafromlocal.h"
#include "ui_datafromlocal.h"

DataFromLocal::DataFromLocal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataFromLocal)
{
    ui->setupUi(this);
    setStyleSheet("font:11pt ;background-color: rgb( 90, 90, 90);color: white;");    //字体大小颜色
    //mydatabase = nullptr;
    mydatabase = new MyDataBase("localhost","Data/test1.db");
}

DataFromLocal::~DataFromLocal()
{
    delete ui;
    if(nullptr != mydatabase)
    {
        delete mydatabase;
    }
}

// 选择文件
void DataFromLocal::on_choseFile_clicked()
{
    QFileDialog *fileDialog=new QFileDialog();
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setNameFilter(("file(*.csv *.txt)"));  //*.xls *.xlsx
    QString openPath=QCoreApplication::applicationDirPath();
    fileDialog->setDirectory(openPath);
    if(fileDialog->exec()==QDialog::Accepted)
    {
        QString srcFile=fileDialog->selectedFiles()[0];
        ui->lineEdit->setText(srcFile);
    }

}

// 将选中文件导入数据库
void DataFromLocal::on_toDatabase_clicked()
{
    qDebug()<<QString::fromLocal8Bit("数据导入开始");
    QString srcPath=ui->lineEdit->text();
    if(srcPath.isEmpty())
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("导入异常"),QString::fromLocal8Bit("路径为空，请确认!"),QMessageBox::Ok);
        return;
    }
    QStringList fileList;
    fileList.clear();
    fileList = srcPath.split("/");    //路径按"/"切分
    QString filename = fileList.at(fileList.size()-1).split(".").at(0) ;    //获得csv表格的文件名
    //判断表格是否存在
    if(mydatabase->IsExist(filename))
    {
        qDebug()<<QString::fromLocal8Bit("同名表格已经存在");
        return;
    }

    QFile file(srcPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 文件打开，一行行写入数据库
        QStringList firstList;     //第一行
        firstList.clear();
        QTextStream in(&file);
        // 第一行
        QString fileLine = in.readLine();
        firstList = fileLine.split(",");    //csv格式以分号间隔
        //qDebug()<< dataList.size();
        data_name.clear();
        data_name.insert("id", "int primary key");  //data_name.insert("id", "int primary key autoincrement");
        for(int i=0;i<firstList.size();i++)
        {
            data_name.insert(firstList.at(i),"varchar");     //不同的表格是不同的数据格式
        }
        //创建表
        mydatabase->create_table(filename,data_name);

        // 开始启动事务
        mydatabase->m_db.transaction();
        //插入记录
        QTime t;
        t.start();
        int lineNumber = 0;
        while(!in.atEnd())
        {
            lineNumber++;
            data_name.clear();
            fileLine = in.readLine();
            QStringList dataList;
            dataList.clear();
            dataList = fileLine.split(",");
            for(int i=0;i<firstList.size();i++)
            {
                data_name.insert("id",QString::number(lineNumber));
                data_name.insert(firstList.at(i),dataList.at(i));     //不同的表格是不同的数据格式
            }
            mydatabase->add(filename,data_name);
        }
        // 提交事务
        mydatabase->m_db.commit();
        qDebug()<<QObject::tr("insert %1 pieces of data into database spend: %2 ms").arg(lineNumber).arg(t.elapsed());
        // 关闭文件
        file.close();
    }
    else
    {
        QMessageBox::warning(this, QString::fromLocal8Bit("导入异常"),QString::fromLocal8Bit("文件打开错误，请确认!"),QMessageBox::Ok);
        qDebug()<< "Cannot open file for reading"
                << qPrintable(file.errorString());
    }
    qDebug()<<QString::fromLocal8Bit("数据导入完成");
}
