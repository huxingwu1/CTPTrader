#ifndef DATAENGINE_H
#define DATAENGINE_H

#include <QMap>
#include <QSet>
#include <QStringList>

#include "publicstruct.h"
#include "positionbuffer.h"
#include "ThostFtdcUserApiStruct.h"

/*
 * publicstruct.h存储了数据结构体
 * DataEngine定义了多种形式的字典，两个成员函数获得指定的信息
 */

// 存储模块公共数据
class DataEngine
{
public:
    bool de_get_contract(QString vtSymbol, InstrumentInfo& contract);
    bool de_get_order(QString ordID, OrderInfo& ordInfo);


public:
    /***MD Interface***/
    // Tick回调（tick需要提炼一下）
    QMap <QString, CThostFtdcDepthMarketDataField> lastMarketDataSet;

    /***TD Interface***/
    // 账户信息
    AccountInfo accountInfo;

    // 持仓查询缓存（区分昨仓和今仓）<key: vtSymbol.持仓方向>，本地所维护的真正持仓数据
    QMap<QString, CPositionBuffer>	allPosition_buffer;     //CPositionBuffer账户持仓信息

    // 持仓查询 <key: vtSymbol.持仓方向>，仅用于用户持仓列表展示的持仓备份
    QMap<QString, PositionInfo>		allPosition;        //PositionInfo持仓信息结构体

    // 存放所有合约信息，供UI界面展示
    QMap <QString, InstrumentInfo>	allInstruments;     //InstrumentInfo合约信息

    // 交易开启后的所有委托
    QMap<QString, OrderInfo>		allOrderDict;       //OrderInfo报单信息

    // 交易活动委托
    QMap<QString, OrderInfo>		workingOrderDict;

    // 成交回报
    QMap<QString, TradeInfo>		finishedOrderDict;

    // 订阅合约信息
    QSet<QString> subscribeInstruments;
};


#endif // DATAENGINE_H
