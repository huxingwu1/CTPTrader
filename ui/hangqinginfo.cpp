#include "hangqinginfo.h"
#include "mainengine.h"
#include "mainwindow.h"

extern MainEngine *me;
//extern TradeboardInfo *tradeboard;
extern MainWindow *w;


HangqingInfo::HangqingInfo(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::TopDockWidgetArea);
    this->DockInit(0,0,900,200);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("合约代码"));
    headerName.append(QString::fromLocal8Bit("时间"));
    headerName.append(QString::fromLocal8Bit("昨收盘价"));
    headerName.append(QString::fromLocal8Bit("开盘价"));
    headerName.append(QString::fromLocal8Bit("最高价"));
    headerName.append(QString::fromLocal8Bit("最低价"));
    headerName.append(QString::fromLocal8Bit("最新价"));
    headerName.append(QString::fromLocal8Bit("涨跌幅(%)"));
    headerName.append(QString::fromLocal8Bit("买一价"));
    headerName.append(QString::fromLocal8Bit("买一量"));
    headerName.append(QString::fromLocal8Bit("卖一价"));
    headerName.append(QString::fromLocal8Bit("卖一量"));
    headerName.append(QString::fromLocal8Bit("成交量"));
    headerName.append(QString::fromLocal8Bit("持仓量"));
    headerName.append(QString::fromLocal8Bit("日期"));
    headerName.append(QString::fromLocal8Bit("接口"));
    this->addTable(900,200,firstTable,headerName);


    // MV模式中定时器自动刷新行情报价
    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(false);
    //connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    connect(updateTimer,&QTimer::timeout,[=](){
        updateData();
    });
    updateTimer->start(2000); // 页面行情报价3秒快照刷新

}

HangqingInfo::~HangqingInfo()
{

}

void HangqingInfo::updateData()
{
    //qDebug()<<"HangqingInfo::updateData";
    initData();
    refrushTable(grid_data_list);
}

void HangqingInfo::initData()
{
    grid_data_list.clear();
    QMap <QString, CThostFtdcDepthMarketDataField>& market_set = me->me_getLastMarketData();

    QMap <QString, CThostFtdcDepthMarketDataField>::iterator it;
    for (it = market_set.begin(); it != market_set.end(); ++it)
    {
        QuoteInfo qm;
        qm.symbol = (it->InstrumentID);
        //qm.vtSymbol = (me->getInstrumentInfo()[it->InstrumentID].getName());
        qm.lastPrice = (it->LastPrice);
        qm.preClosePrice = (it->PreClosePrice);
        qm.volume = (it->Volume);
        qm.openInterest = (it->OpenInterest);
        qm.openPrice = (it->OpenPrice);
        qm.highPrice = (it->HighestPrice);
        qm.lowPrice = (it->LowestPrice);
        qm.bidPrice1 = (it->BidPrice1);
        qm.bidVolume1 = (it->BidVolume1);
        qm.askPrice1 = (it->AskPrice1);
        qm.askVolume1 = (it->AskVolume1);
        qm.upperLimit =(it->UpperLimitPrice);
        qm.lowerLimit =(it->LowerLimitPrice);
        //qm.setTime(QString(QLatin1String(it->UpdateTime) + "%1%2").arg(":").arg(QString::number(it->UpdateMillisec)));
        qm.time = (it->UpdateTime);
        qm.tradingDay = (it->TradingDay);
        qm.gatewayName = ("CTP");
        //计算涨幅
        qm.changePercent = (it->LastPrice - it->PreClosePrice)*100 /it->PreClosePrice;

        grid_data_list.append(qm);
    }
}

void HangqingInfo::refrushTable(QList<QuoteInfo> &all_position)
{
    //清空持仓表
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    //将持仓写入表中
    for(int row = 0; row< all_position.size(); row++)
    {
        firstTable->insertRow(row);
        firstTable->setItem(row, 0, new QTableWidgetItem(all_position.at(row).symbol));
        firstTable->setItem(row, 1, new QTableWidgetItem(all_position.at(row).time));
        firstTable->setItem(row, 2, new QTableWidgetItem(QString::number(all_position.at(row).preClosePrice)));
        firstTable->setItem(row, 3, new QTableWidgetItem(QString::number(all_position.at(row).openPrice)));   //开
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::number(all_position.at(row).highPrice)));
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(all_position.at(row).lowPrice)));
        firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(all_position.at(row).lastPrice)));  //最新价
        firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(all_position.at(row).changePercent,'f',3 )));
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::number(all_position.at(row).bidPrice1)));
        firstTable->setItem(row, 9, new QTableWidgetItem(QString::number(all_position.at(row).bidVolume1)));
        firstTable->setItem(row, 10, new QTableWidgetItem(QString::number(all_position.at(row).askPrice1)));
        firstTable->setItem(row, 11, new QTableWidgetItem(QString::number(all_position.at(row).askVolume1)));
        firstTable->setItem(row, 12, new QTableWidgetItem(QString::number(all_position.at(row).volume)));   //成交量
        firstTable->setItem(row, 13, new QTableWidgetItem(QString::number(all_position.at(row).openInterest)));   //持仓
        firstTable->setItem(row, 14, new QTableWidgetItem(all_position.at(row).tradingDay));
        firstTable->setItem(row, 15, new QTableWidgetItem(all_position.at(row).gatewayName));

        if(all_position.at(row).changePercent>0)
        {
            firstTable->item(row,7)->setBackgroundColor(Qt::red);
        }
        else if(all_position.at(row).changePercent<0)
        {
            firstTable->item(row,7)->setBackgroundColor(Qt::green);
        }

        //将行情数据导入交易面板中
        if(all_position.at(row).symbol == w->tradeboard->tradeboard.Instrument->text())
        {
            w->tradeboard->tradeboard.UpperLimitPrice_show->setText(QString::number(all_position.at(row).upperLimit));
            w->tradeboard->tradeboard.AskPrice1_vol_show->setText(QString::number(all_position.at(row).askVolume1));
            w->tradeboard->tradeboard.AskPrice1_show->setText(QString::number(all_position.at(row).askPrice1));
            w->tradeboard->tradeboard.newPrice1_show->setText(QString::number(all_position.at(row).lastPrice));
            w->tradeboard->tradeboard.BidPrice1_show->setText(QString::number(all_position.at(row).bidPrice1));
            w->tradeboard->tradeboard.BidPrice1_vol_show->setText(QString::number(all_position.at(row).bidVolume1));
            w->tradeboard->tradeboard.LowerLimitPrice_show->setText(QString::number(all_position.at(row).lowerLimit));

        }
    }
}
