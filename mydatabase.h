#ifndef MYDATABASE_H
#define MYDATABASE_H

#include <QTextCodec>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTime>
#include <QSqlError>
#include <QDebug>
#include <QSqlDriver>
#include <QSqlRecord>

class MyDataBase
{
public:
    MyDataBase() = delete;
    MyDataBase(QString HostName, QString DatabaseName, QString UserNmae, QString Password);
    ~MyDataBase();

public:
    //创建表
    bool create_table(QString table_name,QMap<QString,QString> table_data);
    //发送数据库语句
    bool db_query(QString m_query_sql);

    bool add(QString table_name,QMap<QString,QString> data); //增加数据
    bool del(QString table_name,QMap<QString,QString> where); //删除一条记录
    bool updata(QString table_name,QMap<QString,QString> where,QMap<QString,QString> data);  //更新数据
    bool find(QString table_name,QList<QString> key,QMap<QString,QString> where,QList<QList<QString>> *row);    //查找
    bool find(QString table_name,QList<QString> key,QList<QList<QString>> *row);    //查找所有
    QString getError();

private:
    QSqlDatabase m_db;
    QString m_HostName;
    QString m_DatabaseName;
    QString m_UserNmae;
    QString m_Password;
    QSqlError m_error;
    QSqlRecord m_rec;
    QSqlQuery * m_query;
};

#endif // MYDATABASE_H
