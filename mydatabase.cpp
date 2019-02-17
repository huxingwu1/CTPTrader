#include "mydatabase.h"


MyDataBase::MyDataBase(QString HostName, QString DatabaseName)
{
    QString con_path = "Data/";      //默认在Data文件夹下面

    QDir temp;
    if (!temp.exists(con_path))
    {
        bool n = temp.mkpath(con_path);   //Creates the directory path dirPath.
    }


    m_HostName = HostName;
    m_DatabaseName = DatabaseName;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(m_HostName);
    m_db.setDatabaseName(m_DatabaseName);
    m_query = new QSqlQuery(m_db);
    if(!m_db.open())
    {
        qDebug()<<"database open error.";
        m_error = m_db.lastError();
    }
    else
    {
        qDebug()<<"database open successful.";
    }
}

MyDataBase::MyDataBase(QString HostName, QString DatabaseName, QString UserNmae, QString Password)
{
    m_HostName = HostName;
    m_DatabaseName = DatabaseName;
    m_UserNmae = UserNmae;
    m_Password = Password;
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setHostName(m_HostName);
    m_db.setDatabaseName(m_DatabaseName);
    m_db.setUserName(m_UserNmae);
    m_db.setPassword(m_Password);
    m_query = new QSqlQuery(m_db);
    if(!m_db.open())
    {
        qDebug()<<"database open error.";
        m_error = m_db.lastError();
    }
}

MyDataBase::~MyDataBase()
{
    m_db.close();
    delete m_query;
}

/*
 *
 * create_table函数：创建数据库表
 * table_name:表名
 * table_data:表项 名->属性
 *
 */

bool MyDataBase::create_table(QString table_name, QMap<QString, QString> table_data)
{
    QSqlQuery query;
    QString sql="create table "+table_name+" (";
    for(QMap<QString,QString>::const_iterator i=table_data.constBegin();i!=table_data.constEnd();i++)
    {
        sql += i.key()+' '+i.value();
        if(i!=table_data.constEnd())
            sql+=',';
    }
    sql.chop(1);
    sql+=")";
    qDebug()<<sql;
    query.prepare(sql);
    return query.exec();

}

bool MyDataBase::db_query(QString m_query_sql)
{
    m_query->prepare(m_query_sql);
    return m_query->exec();

}

bool MyDataBase::add(QString table_name, QMap<QString, QString> data)
{
    QString sql="insert into "+table_name+  "(";
    QString values=" values(";
    for(QMap<QString,QString>::const_iterator i=data.constBegin();i!=data.constEnd();i++)
    {
        sql+=i.key()+", ";
        values+="'"+ i.value()+ "'" +", ";
    }
    sql.chop(2);
    values.chop(2);
    sql+=")";
    values+=")";
    sql+=values;

//    QString sql="insert into "+table_name+ " values(";
//    for(QMap<QString,QString>::const_iterator i=data.constBegin();i!=data.constEnd();i++)
//    {
//        sql+= "'"+i.value()+"'"+ ", ";
//    }
//    sql.chop(2);
//    sql+=")";
//    qDebug()<<sql;
    m_query->prepare(sql);
    bool ret = m_query->exec();
    if(!ret)
    {
        m_error = m_query->lastError();
        qDebug()<<m_error.driverText()<<QString(QObject::tr("数据插入失败"));
    }
    return ret;

}

bool MyDataBase::del(QString table_name, QMap<QString, QString> where)
{
    QString sql = "delete ";
    sql+= table_name;
    sql+=" where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+="'"+i.value()+"' ";
    }
    sql.chop(2);
    m_query->prepare(sql);
    return m_query->exec();

}

bool MyDataBase::updata(QString table_name, QMap<QString, QString> where, QMap<QString, QString> data)
{
    QString sql="updata "+table_name+" set";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+=i.value()+" ";
    }
    sql+="where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"=";
        sql+=i.value()+" ";
    }
    return m_query->exec();

}

bool MyDataBase::find(QString table_name, QList<QString> key, QMap<QString, QString> where, QList<QList<QString> > *row)
{
    QString sql="select ";
    int len=key.size();
    for(int i=0;i<len;i++)
    {
        sql+=key.at(i);
        sql+=",";
    }
    sql.chop(1);
    sql+=" from "+table_name;
    sql+=" where ";
    for(QMap<QString,QString>::const_iterator i=where.constBegin();i!=where.constEnd();i++)
    {
        sql+=i.key()+"="+i.value()+",";
    }
    sql.chop(1);
    //qDebug()<<sql;
    m_query->prepare(sql);
    if(m_query->exec())
    {
        while (m_query->next()) {
            QList<QString> j;
            for(int i=0;i<len;i++)
            {
                j.append(m_query->value(i).toString());
            }
            row->append(j);
        }
        return 1;
    }
    else return 0;

}

bool MyDataBase::find(QString table_name, QList<QString> key, QList<QList<QString> > *row)
{
    QString sql="select ";
    int len=key.size();
    for(int i=0;i<len;i++)
    {
        sql+=key.at(i);
        sql+=",";
    }
    sql.chop(1);
    sql+=" from "+table_name;
    //qDebug()<<sql;
    m_query->prepare(sql);
    if(m_query->exec())
    {
        while (m_query->next()) {
            QList<QString> j;
            for(int i=0;i<len;i++)
            {
                j.append(m_query->value(i).toString());
            }
            row->append(j);
        }
        return 1;
    }
    else return 0;

}

bool MyDataBase::IsExist(QString tableName)
{
    m_query->exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg(tableName));
    if(m_query->next())
    {
        if(m_query->value(0).toInt()==0)
        {
            // 表不存在
            return false;
        }else{
            return true;
        }
    }
}

QString MyDataBase::getError()
{
    return m_error.text();
}
