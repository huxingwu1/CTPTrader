#include "chicanginfo.h"
#include "mainengine.h"

extern MainEngine *me;


ChicangInfo::ChicangInfo(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    this->DockInit(0,0,600,180);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("合约代码"));
    headerName.append(QString::fromLocal8Bit("名 称"));
    headerName.append(QString::fromLocal8Bit("方 向"));
    headerName.append(QString::fromLocal8Bit("持仓总量"));
    headerName.append(QString::fromLocal8Bit("今持仓"));
    headerName.append(QString::fromLocal8Bit("昨持仓"));
    headerName.append(QString::fromLocal8Bit("冻结量"));
    headerName.append(QString::fromLocal8Bit("价格"));
    headerName.append(QString::fromLocal8Bit("持仓盈亏"));
    headerName.append(QString::fromLocal8Bit("接口"));
    this->addTable(600,180,firstTable,headerName);

    // 刷新用户持仓信息表格
    me->register_event(EVENT_POSITION_UI, this, &ChicangInfo::updateData);
}

ChicangInfo::~ChicangInfo()
{

}

void ChicangInfo::updateData(Event ev)
{
    //qDebug()<<"ChicangInfo::updateData";
    initData(ev);
    refrushTable(grid_data_list);
}

void ChicangInfo::initData(Event ev)
{
    grid_data_list.clear();
    QMap <QString, PositionInfo>& all_position = me->me_getPositionInfo();
    QMap <QString, PositionInfo>::iterator it;
    for (it = all_position.begin(); it != all_position.end(); ++it)
    {
        grid_data_list.append(*it);
    }
}

void ChicangInfo::refrushTable(QList<PositionInfo>& all_position)
{
    QMap <QString, InstrumentInfo>& all_contracts = me->me_getInstrumentInfo();  //所有的合约

    //清空持仓表
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    //将持仓写入表中
    for(int row = 0; row< all_position.size(); row++)
    {
        //合约的合约乘数
        int multiplier = all_contracts[all_position.at(row).symbol].multiplier;
        firstTable->insertRow(row);
        firstTable->setItem(row, 0, new QTableWidgetItem(all_position.at(row).symbol));
        firstTable->setItem(row, 1, new QTableWidgetItem(all_position.at(row).vtSymbol));

        if('2' == all_position.at(row).direction)  //0买入
        {
            firstTable->setItem(row, 2, new QTableWidgetItem(QString::fromLocal8Bit("买 多")));
            firstTable->item(row,2)->setBackgroundColor(Qt::red);
            MainEngine::AskBidPrice askbid = me->me_getInstrumentPrice(all_position.at(row).symbol);
            if((askbid.new1 -0.0) < 0.000001)
            {
                firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("获取最新价失败")));
            }
            else
            {
                double profit = (askbid.new1 - all_position.at(row).price) * multiplier * (all_position.at(row).position);
                firstTable->setItem(row, 8, new QTableWidgetItem(QString::number(profit)));
                if(profit >0)
                {
                    firstTable->item(row,8)->setBackgroundColor(Qt::red);
                }
                else if(profit <0)
                {
                    firstTable->item(row,8)->setBackgroundColor(Qt::green);
                }
            }
        }
        else if('3' == all_position.at(row).direction)  //1卖出
        {
            firstTable->setItem(row, 2, new QTableWidgetItem(QString::fromLocal8Bit("卖 空")));
            firstTable->item(row,2)->setBackgroundColor(Qt::green);
            MainEngine::AskBidPrice askbid = me->me_getInstrumentPrice(all_position.at(row).symbol);
            if((askbid.new1 -0.0) < 0.000001)
            {
                firstTable->setItem(row, 8, new QTableWidgetItem(QString::fromLocal8Bit("获取最新价失败")));
            }
            else
            {
                double profit = (all_position.at(row).price - askbid.new1) * multiplier * (all_position.at(row).position);
                firstTable->setItem(row, 8, new QTableWidgetItem(QString::number(profit)));
                if(profit >0)
                {
                    firstTable->item(row,8)->setBackgroundColor(Qt::red);
                }
                else if(profit <0)
                {
                    firstTable->item(row,8)->setBackgroundColor(Qt::green);
                }
            }
        }
        else
        {
            firstTable->setItem(row, 2, new QTableWidgetItem(QString::fromLocal8Bit("净")));
        }
//        firstTable->setItem(row, 2, new QTableWidgetItem(all_position.at(row).direction));
        firstTable->setItem(row, 3, new QTableWidgetItem(QString::number(all_position.at(row).position)));
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::number(all_position.at(row).position - all_position.at(row).ydPosition)));
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(all_position.at(row).ydPosition)));
        firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(all_position.at(row).frozen)));
        firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(all_position.at(row).price)));
        firstTable->setItem(row, 9, new QTableWidgetItem(all_position.at(row).gatewayName));

        //更新持仓表时订阅合约，订阅合约不会重复订阅
        me->me_subscribe(all_position.at(row).symbol);
        QThread::msleep(50);
    }

}
