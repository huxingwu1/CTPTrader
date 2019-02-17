#ifndef MAINENGINE_H
#define MAINENGINE_H

#include <QObject>
#include <atomic>
#include <fstream>
#include <QMutex>
#include <QString>
#include <QDateTime>
#include <QDir>
#include <QList>
#include "eventengine.h"
#include "dataengine.h"
#include "ctaengine.h"
#include "ctpmdapi.h"
#include "ctptdapi.h"
#include "smtp.h"


// 框架对外主引擎,管理框架对外的所有公共交易和查询，内部实现不同接口的命令转发
class MainEngine : public QObject
{
    Q_OBJECT
public:
    MainEngine(QObject *parent = nullptr);
    ~MainEngine();
    //日志函数
    void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

    template<typename T>
    void register_event(std::string type, T* pObj, void (T::*pMemberFunc)(Event))
    {
        ee->addEvent(type, pObj, pMemberFunc);
    }

signals:
    void sendLog(QString);

public:
    // 登录
    void me_login(QString userid, QString password, QString brokerid, QString mdAddress, QString tdAddress);
    // 多账户登录
    void me_login(QString userid, QString password, QString brokerid, QString tdAddress);
    // 退出
    void me_logout();
    // 订阅行情
    void me_subscribe(QString instrumentid);

    // 用户登录标记
    bool me_get_is_login();

    // 委托单动作
    QString me_sendDefaultOrder(orderCommonRequest& order_field);
    void me_cancelOrder(cancelCommonRequest& cancel_field);

    // CTA测试
    void me_strat_cta();
    void me_stop_cta();

    // CTA下单查询合约和委托单
    bool me_get_contract(QString vtSymbol, InstrumentInfo& contract);
    bool me_get_order(QString ordID, OrderInfo& ordInfo);
    // 发送邮件
    void sendEmail(QString theme, QString content);

    // 从de->allInstruments中获得合约instrument最新的价格
    typedef struct AskBidPrice
    {
        double ask1;  //卖一
        double new1;
        double bid1;
    }AskBidPrice;
    // 获得合约最新的价格
    AskBidPrice me_getInstrumentPrice(QString instruments);

    // 查询动作：用户交易情况基本信息查询,主要用于UI界面刷新，后期UI改造要去掉
    //账户
    AccountInfo me_getAccountInfo() const;
    //持仓
    QMap<QString, PositionInfo> me_getPositionInfo() const;
    //合约信息
    QMap<QString, InstrumentInfo> me_getInstrumentInfo() const;
    //获得上一个市场数据
    QMap<QString, CThostFtdcDepthMarketDataField> me_getLastMarketData() const;
    //获得委托
    QMap<QString, OrderInfo> me_getOrderInfo() const;
    QMap<QString, OrderInfo> me_getWorkingOrderInfo() const;
    //获得成交回报
    QMap<QString, TradeInfo> me_getfinishedOrderInfo() const;
private:
    std::atomic<bool> me_is_login{ false };

    // 扩展引擎模块
    EventEngine* ee;
    DataEngine* de;
    std::shared_ptr<CtaEngine> ce;

    // Gateway接口
    CtpMdApi* ctpmd;
    CtpTdApi* ctptd;

    QList <CtpTdApi*> ctptdGateway;  //添加多账户存放在ctptdGateway中
public:
    std::ofstream g_OutputDebug;
    bool isEmail {false};       //是否配置邮箱
    struct EmailInfo
    {
        QString server;
        QString sendAddress;
        QString password;
        QString receviceAddress;
    }emailinfo;
    QList<Smtp*> emailList;
};

#endif // MAINENGINE_H
