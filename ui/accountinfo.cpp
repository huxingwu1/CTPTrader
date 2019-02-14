#include "accountinfo.h"
#include "mainengine.h"

extern MainEngine *me;

MyAccountInfo::MyAccountInfo(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    this->DockInit(0,0,600,180);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("账户"));
    headerName.append(QString::fromLocal8Bit("昨结"));
    headerName.append(QString::fromLocal8Bit("净值"));
    headerName.append(QString::fromLocal8Bit("可用"));
    headerName.append(QString::fromLocal8Bit("手续费"));
    headerName.append(QString::fromLocal8Bit("保证金"));
    headerName.append(QString::fromLocal8Bit("平仓盈亏"));
    headerName.append(QString::fromLocal8Bit("持仓盈亏"));
    headerName.append(QString::fromLocal8Bit("风险度(%)"));
    headerName.append(QString::fromLocal8Bit("接口"));
    this->addTable(600,180,firstTable,headerName);

    // 刷新用户资金信息表格
    me->register_event(EVENT_ACCOUNT, this, &MyAccountInfo::updateData);
}

MyAccountInfo::~MyAccountInfo()
{

}

void MyAccountInfo::updateData(Event ev)
{
    //qDebug()<<"MyAccountInfo::updateData";
    initData(ev);
    refrushTable(grid_data_list);   //QTableWidget刷新
}

void MyAccountInfo::initData(Event ev)
{
    grid_data_list.clear();
    AccountInfo& account_info = me->me_getAccountInfo();
    grid_data_list.append(account_info);
}

void MyAccountInfo::refrushTable(QList<AccountInfo> &rowlist)
{
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    int row = firstTable->rowCount();
    firstTable->insertRow(row);
    firstTable->setItem(row, 0, new QTableWidgetItem(rowlist.at(0).id));
    firstTable->setItem(row, 1, new QTableWidgetItem(QString::number(rowlist.at(0).preBalance,10,2)));
    firstTable->setItem(row, 2, new QTableWidgetItem(QString::number(rowlist.at(0).balance,10,2)));
    firstTable->setItem(row, 3, new QTableWidgetItem(QString::number(rowlist.at(0).available,10,2)));
    firstTable->setItem(row, 4, new QTableWidgetItem(QString::number(rowlist.at(0).commission,10,2)));
    firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(rowlist.at(0).margin,10,2)));
    firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(rowlist.at(0).close_profit,10,2)));
    firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(rowlist.at(0).position_profit,10,2)));
    firstTable->setItem(row, 8, new QTableWidgetItem(QString::number(rowlist.at(0).margin/rowlist.at(0).balance *100,10,2)));
    firstTable->item(row,8)->setBackgroundColor(Qt::red);
    firstTable->setItem(row, 9, new QTableWidgetItem(rowlist.at(0).gatewayName));
}
