#include "weituoinfo.h"
#include "mainengine.h"

extern MainEngine *me;


WeituoInfo::WeituoInfo(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->DockInit(0,0,900,200);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("委托编号"));
    headerName.append(QString::fromLocal8Bit("合约代码"));
    headerName.append(QString::fromLocal8Bit("交易所"));
    headerName.append(QString::fromLocal8Bit("方向"));
    headerName.append(QString::fromLocal8Bit("开平"));
    headerName.append(QString::fromLocal8Bit("价格"));
    headerName.append(QString::fromLocal8Bit("委托数量"));
    headerName.append(QString::fromLocal8Bit("成交数量"));
    headerName.append(QString::fromLocal8Bit("状态"));
    headerName.append(QString::fromLocal8Bit("委托时间"));
    headerName.append(QString::fromLocal8Bit("撤销时间"));
    headerName.append(QString::fromLocal8Bit("前置编号"));
    headerName.append(QString::fromLocal8Bit("会话编号"));
    headerName.append(QString::fromLocal8Bit("接口"));
    this->addTable(900,200,firstTable,headerName);

    // 刷新用户持仓信息表格
    me->register_event(EVENT_ORDER, this, &WeituoInfo::updateData);
}

WeituoInfo::~WeituoInfo()
{

}

void WeituoInfo::updateData(Event ev)
{
    OrderInfo* quote = (OrderInfo*)ev.data_.get();
    //qDebug()<<quote->orderID<<"  "<<quote->status<<" "<<quote->orderID;
    for(int i=0 ;i<firstTable->rowCount(); i++)
    {
        if (firstTable->item(i, 0)->text() == quote->orderID)  //判断委托回报是否在表中
        {
            firstTable->setItem(i, 0, new QTableWidgetItem(quote->orderID));
            firstTable->setItem(i, 1, new QTableWidgetItem(quote->symbol));
            firstTable->setItem(i, 2, new QTableWidgetItem(quote->exchange));
            if('0' == quote->direction)  //0买入
            {
                firstTable->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("买 多")));
                firstTable->item(i,3)->setBackgroundColor(Qt::red);
            }
            else if('1' == quote->direction)  //1卖出
            {
                firstTable->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("卖 空")));
                firstTable->item(i,3)->setBackgroundColor(Qt::green);
            }
            else
            {
                firstTable->setItem(i, 3, new QTableWidgetItem(QString::fromLocal8Bit("uncertain direction")));
                qDebug()<<QString::fromLocal8Bit("订单回报买卖方向不确定");
            }

            if('0' == quote->offset)
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("开 仓")));
            }
            else if('1' == quote->offset)
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 仓")));
            }
            else if('2' == quote->offset)
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("强 平")));
            }
            else if('3' == quote->offset)
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 今")));
            }
            else if('4' == quote->offset)
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 昨")));
            }
            else
            {
                firstTable->setItem(i, 4, new QTableWidgetItem(QString::fromLocal8Bit("uncertain offset")));
                qDebug()<<QString::fromLocal8Bit("订单回报开平方向不确定");
            }
            firstTable->setItem(i, 5, new QTableWidgetItem(QString::number(quote->price)));
            firstTable->setItem(i, 6, new QTableWidgetItem(QString::number(quote->totalVolume)));
            firstTable->setItem(i, 7, new QTableWidgetItem(QString::number(quote->tradeVolume)));
            if('0' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("全部成交")));
            }
            else if('1' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交还在队列")));
                firstTable->item(i,8)->setBackgroundColor(Qt::blue);
            }
            else if('2' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交不在队列")));
            }
            else if('3' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交还在队列")));
                firstTable->item(i,8)->setBackgroundColor(Qt::blue);
            }
            else if('4' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交不在队列")));
            }
            else if('5' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("已撤单")));
            }
            else if('a' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("未知")));
            }
            else if('b' == quote->status)
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("尚未触发")));
            }
            else
            {
                firstTable->setItem(i, 8, new QTableWidgetItem(QString::fromLocal8Bit("已触发")));
            }
            firstTable->setItem(i, 9, new QTableWidgetItem(quote->orderTime));
            firstTable->setItem(i, 10, new QTableWidgetItem(quote->cancelTime));
            firstTable->setItem(i, 11, new QTableWidgetItem(QString::number(quote->frontID)));
            firstTable->setItem(i, 12, new QTableWidgetItem(QString::number(quote->sessionID)));
            firstTable->setItem(i, 13, new QTableWidgetItem(quote->gatewayName));
            return;
        }
    }
    // 不在表中就插入
    int row =firstTable->rowCount();
    firstTable->insertRow(row);
    firstTable->setItem(row, 0, new QTableWidgetItem(quote->orderID));
    firstTable->setItem(row, 1, new QTableWidgetItem(quote->symbol));
    firstTable->setItem(row, 2, new QTableWidgetItem(quote->exchange));
    if('0' == quote->direction)  //0买入
    {
        firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("买 多")));
        firstTable->item(row,3)->setBackgroundColor(Qt::red);
    }
    else if('1' == quote->direction)  //1卖出
    {
        firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("卖 空")));
        firstTable->item(row,3)->setBackgroundColor(Qt::green);
    }
    else
    {
        firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("uncertain direction")));
        qDebug()<<QString::fromLocal8Bit("订单回报买卖方向不确定");
    }

    if('0' == quote->offset)
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("开 仓")));
    }
    else if('1' == quote->offset)
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 仓")));
    }
    else if('2' == quote->offset)
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("强 平")));
    }
    else if('3' == quote->offset)
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 今")));
    }
    else if('4' == quote->offset)
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 昨")));
    }
    else
    {
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("uncertain offset")));
        qDebug()<<QString::fromLocal8Bit("订单回报开平方向不确定");
    }
    firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(quote->price)));
    firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(quote->totalVolume)));
    firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(quote->tradeVolume)));
    if('0' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("全部成交")));
    }
    else if('1' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交还在队列")));
        firstTable->item(row,8)->setBackgroundColor(Qt::blue);
    }
    else if('2' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交不在队列")));
    }
    else if('3' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交还在队列")));
        firstTable->item(row,8)->setBackgroundColor(Qt::blue);
    }
    else if('4' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交不在队列")));
    }
    else if('5' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("已撤单")));
    }
    else if('a' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未知")));
    }
    else if('b' == quote->status)
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("尚未触发")));
    }
    else
    {
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("已触发")));
    }
    firstTable->setItem(row, 9, new QTableWidgetItem(quote->orderTime));
    firstTable->setItem(row, 10, new QTableWidgetItem(quote->cancelTime));
    firstTable->setItem(row, 11, new QTableWidgetItem(QString::number(quote->frontID)));
    firstTable->setItem(row, 12, new QTableWidgetItem(QString::number(quote->sessionID)));
    firstTable->setItem(row, 13, new QTableWidgetItem(quote->gatewayName));
}

// 之前版本
//void WeituoInfo::updateData(Event ev)
//{
//    //qDebug()<<"WeituoInfo::updateData";
//    initData(ev);
//    refrushTable(grid_data_list);
//}

void WeituoInfo::initData(Event ev)
{
    grid_data_list.clear();

    QMap <QString, OrderInfo>& all_order = me->me_getOrderInfo();// me_getWorkingOrderInfo(); 有效委托   me_getOrderInfo();所有委托

    QMap <QString, OrderInfo>::iterator it;
    for (it = all_order.begin(); it != all_order.end(); ++it)
    {
        grid_data_list.append(*it);
    }
}

//"委托编号","合约代码","名称","方向","开平","价格","委托数量",
//"成交数量","状态","委托时间","撤销时间","前置编号","会话编号","接口"
void WeituoInfo::refrushTable(QList<OrderInfo> &all_order)
{
    //清空持仓表
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    //将持仓写入表中
    for(int row = 0; row< all_order.size(); row++)
    {
        firstTable->insertRow(row);
        firstTable->setItem(row, 0, new QTableWidgetItem(all_order.at(row).orderID));
        firstTable->setItem(row, 1, new QTableWidgetItem(all_order.at(row).symbol));
        firstTable->setItem(row, 2, new QTableWidgetItem(all_order.at(row).exchange));

        if('0' == all_order.at(row).direction)  //0买入
        {
            firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("买 多")));
            firstTable->item(row,3)->setBackgroundColor(Qt::red);
        }
        else if('1' == all_order.at(row).direction)  //1卖出
        {
            firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("卖 空")));
            firstTable->item(row,3)->setBackgroundColor(Qt::green);
        }
        else
        {
            firstTable->setItem(row, 3, new QTableWidgetItem(QString::fromLocal8Bit("uncertain direction")));
            qDebug()<<QString::fromLocal8Bit("订单回报买卖方向不确定");
        }

        if('0' == all_order.at(row).offset)
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("开 仓")));
        }
        else if('1' == all_order.at(row).offset)
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 仓")));
        }
        else if('2' == all_order.at(row).offset)
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("强 平")));
        }
        else if('3' == all_order.at(row).offset)
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 今")));
        }
        else if('4' == all_order.at(row).offset)
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("平 昨")));
        }
        else
        {
            firstTable->setItem(row, 4, new QTableWidgetItem(QString::fromLocal8Bit("uncertain offset")));
            qDebug()<<QString::fromLocal8Bit("订单回报开平方向不确定");
        }
//        firstTable->setItem(row, 3, new QTableWidgetItem(all_order.at(row).direction));
//        firstTable->setItem(row, 4, new QTableWidgetItem(all_order.at(row).offset));
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(all_order.at(row).price)));
        firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(all_order.at(row).totalVolume)));
        firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(all_order.at(row).tradeVolume)));

        if('0' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("全部成交")));
        }
        else if('1' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交还在队列")));
            firstTable->item(row,8)->setBackgroundColor(Qt::blue);
        }
        else if('2' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("部分成交不在队列")));
        }
        else if('3' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交还在队列")));
            firstTable->item(row,8)->setBackgroundColor(Qt::blue);
        }
        else if('4' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未成交不在队列")));
        }
        else if('5' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("已撤单")));
        }
        else if('a' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("未知")));
        }
        else if('b' == all_order.at(row).status)
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("尚未触发")));
        }
        else
        {
            firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("已触发")));
        }
//        firstTable->setItem(row, 8, new QTableWidgetItem(all_order.at(row).status));
        firstTable->setItem(row, 9, new QTableWidgetItem(all_order.at(row).orderTime));
        firstTable->setItem(row, 10, new QTableWidgetItem(all_order.at(row).cancelTime));
        firstTable->setItem(row, 11, new QTableWidgetItem(QString::number(all_order.at(row).frontID)));
        firstTable->setItem(row, 12, new QTableWidgetItem(QString::number(all_order.at(row).sessionID)));
        firstTable->setItem(row, 13, new QTableWidgetItem(all_order.at(row).gatewayName));
        firstTable->setItem(row, 14, new QTableWidgetItem(all_order.at(row).vtOrderID));

    }
}
