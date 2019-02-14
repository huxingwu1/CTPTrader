#include "mainengine.h"


MainEngine::MainEngine(QObject *parent): QObject(parent)
{
    ee = new EventEngine;
    de = new DataEngine;
    ctpmd = new CtpMdApi(ee, de);
    ctptd = new CtpTdApi(ee, de);
    ce = std::move(std::shared_ptr<CtaEngine>(new CtaEngine(this, ee)));

    QString log_path = "Log/";                // + QString(QString(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt"));
    QString filename = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt");
    QDir temp;
    if (!temp.exists(log_path))
    {
        bool n = temp.mkpath(log_path);   //Creates the directory path dirPath.
    }
    //g_OutputDebug.open(qPrintable(log_path.append(QString(QString(QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss").append("-log.txt")) )) , std::ios::out | std::ios::trunc);
    g_OutputDebug.open(qPrintable(log_path + filename) , std::ios::out | std::ios::trunc);

}

void MainEngine::outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString message;
    switch(type)
    {
    case QtDebugMsg:
        message = QString("Debug:");
        break;

    case QtWarningMsg:
        message = QString("Warning:");
        break;

    case QtCriticalMsg:
        message = QString("Critical:");
        break;

    case QtFatalMsg:
        message = QString("Fatal:");
    }
    //browser->append(message.append(msg));
    //    //日志写文件
    //    QFile file("log.txt");
    //    file.open(QIODevice::WriteOnly | QIODevice::Append);
    //    QTextStream text_stream(&file);
    //    text_stream << message.append(msg) << "\r\n";
    //    file.flush();
    //    file.close();

    QString context_info = QString("F:(%1) L:(%2)").arg(QString(context.file)).arg(context.line); // F文件信息L行数
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    std::string message123 = qPrintable(QString("%1 %2 \t%3 \t%4").arg(message).arg(context_info).arg(current_date).arg(msg));
    //std::string message123 = qPrintable(QString("%1 %2 \t%3").arg(message).arg(current_date).arg(msg));

    //写入文件
    g_OutputDebug << message123 << "\r\n"; // std::ofstream
    QString log_info = message +"++" +current_date + "++" +msg;
    emit sendLog(log_info);    //将打印信息发给mainwindow
    // 解锁
    mutex.unlock();
}

void MainEngine::me_login(QString userid, QString password, QString brokerid, QString mdAddress, QString tdAddress)
{
    // 初始化交易和行情接口
    ctpmd->ctp_md_init(mdAddress);
    ctptd->ctp_td_init(tdAddress, userid, password, brokerid);

    // 交易行行情接口登录
    while (true)
    {
        if (ctpmd->get_is_md_connect())
        {
            ctpmd->ctp_md_login();      //md行情登录
            break;
        }
    }
    while (true)
    {
        if (ctptd->get_is_td_connect())
        {
            ctptd->ctp_td_login();      //td行情登录
            break;
        }
    }

    // 开启Event引擎
    ee->ee_begin();
    // 时间触发：暂定查询资金和持仓动作
    register_event(EVENT_TIMER, ctptd, &CtpTdApi::ctp_td_query);

    // MainEngine登录完成标志
    me_is_login.store(true);
}

void MainEngine::me_logout()
{
    // 如果登录完成标记为true才释放资源
    if (me_is_login.load())
    {
        // 注销MD_API
        ctpmd->ctp_md_logout();
        ctpmd->ctp_md_release();

        // 注销TD_API
        ctptd->ctp_td_logout();
        while (!ctptd->get_is_td_logout())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        ctptd->ctp_td_release();

        // 关闭Event引擎
        ee->ee_stop();
    }
}

void MainEngine::me_subscribe(QString instrumentid)
{
    auto iter = de->subscribeInstruments.find(instrumentid);
    if( iter != de->subscribeInstruments.end())
    {
//        qDebug() << QString::fromLocal8Bit("合约:")
//                 << instrumentid
//                 << QString::fromLocal8Bit("已经订阅.");
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("合约:")
                 << instrumentid
                 << QString::fromLocal8Bit("订阅成功.");
        // 订阅行情
        ctpmd->ctp_md_subscribe(instrumentid);
        // 查询费率
        ctptd->ctp_td_getCommission(instrumentid);
        // 添加到订阅列表
        de->subscribeInstruments.insert(instrumentid);
//        for(auto it=de->subscribeInstruments.begin (); it!=de->subscribeInstruments.end ();it++)
//        {
//            qDebug()<< *it;
//        }
    }

}

bool MainEngine::me_get_is_login()
{
    return me_is_login.load();
}

// 默认限价单
QString MainEngine::me_sendDefaultOrder(orderCommonRequest &order_field)
{
    if(me_get_is_login())
    {
        //qDebug()<<QObject::tr("合约: 1%, 价格 %2,数量 %3,方向 %4, 开平%5 下单成功").arg(QString(order_field.instrument)).arg(order_field.price).arg(order_field.volume).arg(order_field.direction).arg(order_field.offset);
        qDebug()<< QString::fromLocal8Bit("合约:")
                << order_field.instrument
                << QString::fromLocal8Bit("价格:")
                << order_field.price
                << QString::fromLocal8Bit("下单成功.");

        return ctptd->ctp_td_send_limitOrder(order_field.instrument,
                                             order_field.price,
                                             order_field.volume,
                                             order_field.direction,
                                             order_field.offset);
    }
}

void MainEngine::me_cancelOrder(cancelCommonRequest &cancel_field)
{
    if(me_get_is_login())
    {
        qDebug()<< QString::fromLocal8Bit("合约:")
                << cancel_field.instrument
                << QString::fromLocal8Bit("撤单成功.");
        ctptd->ctp_td_cancelOrder(cancel_field.instrument,
                                  cancel_field.exchange,
                                  cancel_field.order_ref,
                                  cancel_field.front_id,
                                  cancel_field.session_id);
    }
}

void MainEngine::me_strat_cta()
{
    if(me_get_is_login())
    {
        Event begStgEvent(EVENT_STG_BEG);
        ee->putEvent(begStgEvent);
        qDebug()<<QString::fromLocal8Bit("开启cta策略");
    }
}

void MainEngine::me_stop_cta()
{
    if(me_get_is_login())
    {
        // 此处由Qt线程调用，函数中处理的资源与事件回调线程存在竞争关系，如workingStopOrderDict，故直接做到事件队列
        Event endStgEvent(EVENT_STG_END);
        ee->putEvent(endStgEvent);
        qDebug()<<QString::fromLocal8Bit("停止cta策略");
    }
}

bool MainEngine::me_get_contract(QString vtSymbol, InstrumentInfo &contract)
{
    return de->de_get_contract(vtSymbol, contract);
}

bool MainEngine::me_get_order(QString ordID, OrderInfo &ordInfo)
{
    return de->de_get_order(ordID, ordInfo);
}

MainEngine::AskBidPrice MainEngine::me_getInstrumentPrice(QString instruments)
{
    MainEngine::AskBidPrice askbid;
    QMap <QString, CThostFtdcDepthMarketDataField> MarketData= de->lastMarketDataSet;
    if(MarketData.find(instruments) != MarketData.end())
    {
        askbid.ask1 =MarketData[instruments].AskPrice1;
        askbid.new1 =MarketData[instruments].LastPrice;
        askbid.bid1 =MarketData[instruments].BidPrice1;
    }
    else
    {
        askbid.ask1 = 0;
        askbid.new1 = 0;
        askbid.bid1 = 0;
    }
    return askbid;
}

AccountInfo MainEngine::me_getAccountInfo() const
{
    return de->accountInfo;
}

QMap<QString, PositionInfo> MainEngine::me_getPositionInfo() const
{
    return de->allPosition;
}

QMap<QString, InstrumentInfo> MainEngine::me_getInstrumentInfo() const
{
    return de->allInstruments;
}

QMap<QString, CThostFtdcDepthMarketDataField> MainEngine::me_getLastMarketData() const
{
    return de->lastMarketDataSet;
}

// 交易开启后的所有委托
QMap<QString, OrderInfo> MainEngine::me_getOrderInfo() const
{
    return de->allOrderDict;
}

// 交易活动委托
QMap<QString, OrderInfo> MainEngine::me_getWorkingOrderInfo() const
{
    return de->workingOrderDict;
}

QMap<QString, TradeInfo> MainEngine::me_getfinishedOrderInfo() const
{
    return de->finishedOrderDict;
}
