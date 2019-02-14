#include "historytradeinfo.h"
#include "mainengine.h"

extern MainEngine *me;


HistoryTrade::HistoryTrade(QWidget *parent, QString s) :BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->DockInit(0,0,900,200);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("合约代码"));
    headerName.append(QString::fromLocal8Bit("名称"));
    headerName.append(QString::fromLocal8Bit("交易所"));
    headerName.append(QString::fromLocal8Bit("交易ID"));
    headerName.append(QString::fromLocal8Bit("方向"));
    headerName.append(QString::fromLocal8Bit("开平"));
    headerName.append(QString::fromLocal8Bit("价格"));
    headerName.append(QString::fromLocal8Bit("数量"));
    headerName.append(QString::fromLocal8Bit("交易时间"));
    headerName.append(QString::fromLocal8Bit("接口"));
    this->addTable(900,200,firstTable,headerName);

    // 刷新用户资金信息表格
    me->register_event(EVENT_TRADE, this, &HistoryTrade::updateData);
}

HistoryTrade::~HistoryTrade()
{

}

void HistoryTrade::updateData(Event ev)
{
    TradeInfo* quote = (TradeInfo*)ev.data_.get();
    QString Symbol = quote->symbol;
    int row =firstTable->rowCount();
    firstTable->insertRow(row);
    firstTable->setItem(row, 0, new QTableWidgetItem(quote->symbol));
    firstTable->setItem(row, 1, new QTableWidgetItem(quote->vtSymbol));
    firstTable->setItem(row, 2, new QTableWidgetItem(quote->exchange));
    firstTable->setItem(row, 3, new QTableWidgetItem(quote->tradeID));
    if('0' == quote->direction)  //0买入
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("买 多")));
        firstTable->item(row,4)->setBackgroundColor(Qt::red);
    }
    else if('1' == quote->direction)  //1卖出
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("卖 空")));
        firstTable->item(row,4)->setBackgroundColor(Qt::green);
    }
    else
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("uncertain direction")));
        qDebug()<<QString::fromLocal8Bit("订单成交买卖方向不确定");
    }

    if('0' == quote->offset)
    {
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("开 仓")));
    }
    else if('3' == quote->offset)
    {
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("平 今")));
    }
    else if('4' == quote->offset)
    {
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("平 昨")));
    }
    else
    {
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("uncertain offset")));
        qDebug()<<QString::fromLocal8Bit("订单成交开平方向不确定");
    }
//    firstTable->setItem(row, 4, new QTableWidgetItem(quote->direction));
//    firstTable->setItem(row, 5, new QTableWidgetItem(quote->offset));
    firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(quote->price)));
    firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(quote->volume)));
    firstTable->setItem(row, 8, new QTableWidgetItem(quote->tradeTime));
    firstTable->setItem(row, 9, new QTableWidgetItem(quote->gatewayName));


}
// 1.27之前的写法,从de中取数据全量刷新
//void HistoryTrade::updateData(Event ev)
//{
//    //qDebug()<<"HistoryTrade::updateData";
//    initData(ev);
//    //qDebug()<<grid_data_list.size();
//    refrushTable(grid_data_list);
//}

void HistoryTrade::initData(Event ev)
{
    grid_data_list.clear();

    QMap <QString, TradeInfo>& all_order = me->me_getfinishedOrderInfo();

    QMap <QString, TradeInfo>::iterator it;
    for (it = all_order.begin(); it != all_order.end(); ++it)
    {
        grid_data_list.append(*it);
    }
}


void HistoryTrade::refrushTable(QList<TradeInfo> &finishedOrder)
{
    //清空持仓表
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    //将持仓写入表中
    for(int row = 0; row< finishedOrder.size(); row++)
    {
        firstTable->insertRow(row);
        firstTable->setItem(row, 0, new QTableWidgetItem(finishedOrder.at(row).symbol));
        firstTable->setItem(row, 1, new QTableWidgetItem(finishedOrder.at(row).vtSymbol));
        firstTable->setItem(row, 2, new QTableWidgetItem(finishedOrder.at(row).exchange));
        firstTable->setItem(row, 3, new QTableWidgetItem(finishedOrder.at(row).tradeID));

        if('0' == finishedOrder.at(row).direction)  //0买入
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("买多")));
            firstTable->item(row,4)->setBackgroundColor(Qt::red);
        }
        else if('1' == finishedOrder.at(row).direction)  //1卖出
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("卖空")));
            firstTable->item(row,4)->setBackgroundColor(Qt::green);
        }
        else
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("uncertain direction")));
            qDebug()<<QString::fromLocal8Bit("订单成交买卖方向不确定");
        }

        if('0' == finishedOrder.at(row).offset)
        {
            firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("开仓")));
        }
        else if('3' == finishedOrder.at(row).offset)
        {
            firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("平今")));
        }
        else if('4' == finishedOrder.at(row).offset)
        {
            firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("平昨")));
        }
        else
        {
            firstTable->setItem(row, 5, new QTableWidgetItem(QString::fromLocal8Bit("uncertain offset")));
            qDebug()<<QString::fromLocal8Bit("订单成交开平方向不确定");
        }
//        firstTable->setItem(row, 4, new QTableWidgetItem(finishedOrder.at(row).direction));
//        firstTable->setItem(row, 5, new QTableWidgetItem(finishedOrder.at(row).offset));
        firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(finishedOrder.at(row).price)));
        firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(finishedOrder.at(row).volume)));
        firstTable->setItem(row, 8, new QTableWidgetItem(finishedOrder.at(row).tradeTime));
        firstTable->setItem(row, 9, new QTableWidgetItem(finishedOrder.at(row).gatewayName));

    }
}
