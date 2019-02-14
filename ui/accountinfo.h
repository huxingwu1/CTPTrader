#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"

/*
 * AccountInfo继承包含一个QDockWidget,QTableWidget,headerName
 */

class MyAccountInfo : public BaseView
{
public:
    MyAccountInfo(QWidget *parent,QString s);
    ~MyAccountInfo();


public slots:
    //更新表格
    void updateData(Event ev);

private:
    void initData(Event ev);
    void refrushTable(QList<AccountInfo> &rowlist);
    QList<AccountInfo> grid_data_list;
    //QTimer *updateTimer;

};

#endif // ACCOUNTINFO_H
