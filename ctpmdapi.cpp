#include "ctpmdapi.h"
#include <string>

bool CtpMdApi::get_is_md_connect()
{
    return is_md_connect.load();
}

bool CtpMdApi::get_is_md_logout()
{
    return is_md_logout.load();
}

void CtpMdApi::ctp_md_init(QString mdaddress)
{
    QString con_path = "conn_file/md/";   //md不需要保存在多个账户下

    QDir temp;
    if (!temp.exists(con_path))
    {
        bool n = temp.mkpath(con_path);
    }
    char frontaddress[512] = { 0 };
    strncpy_s(frontaddress, mdaddress.toStdString().c_str(), sizeof(frontaddress));

    MdApi = CThostFtdcMdApi::CreateFtdcMdApi(con_path.toStdString().c_str());
    MdApi->RegisterSpi(this);
    MdApi->RegisterFront(frontaddress);
    MdApi->Init();
}

void CtpMdApi::ctp_md_release()
{
    if (MdApi != nullptr)
    {
        MdApi->RegisterSpi(nullptr);
        MdApi->Release();
        MdApi = nullptr;
    }
}

void CtpMdApi::ctp_md_login()
{
    CThostFtdcReqUserLoginField loginField = { 0 };
    MdApi->ReqUserLogin(&loginField, requestID++);
    qDebug()<<QString::fromLocal8Bit("MD行情登录");      //测试用  md login
}

void CtpMdApi::ctp_md_logout()
{
    CThostFtdcUserLogoutField logoutField = { 0 };
    MdApi->ReqUserLogout(&logoutField, requestID++);
    qDebug()<<QString::fromLocal8Bit("CtpMdApi::ctp_md_logout");
}

void CtpMdApi::ctp_md_subscribe(QString instrumendid)
{
    char id[15] = { 0 };
    strncpy_s(id, instrumendid.toStdString().c_str(), sizeof(id));

    char* insts[] = { id };
    MdApi->SubscribeMarketData(insts, 1);
}

void CtpMdApi::OnFrontConnected()
{
    is_md_connect.store(true);
}

void CtpMdApi::OnFrontDisconnected(int nReason)
{
    is_md_connect.store(false);
}

void CtpMdApi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo->ErrorID == 0)
    {
        // 登录成功
        qDebug() << QString::fromLocal8Bit("MD行情登录成功");  //MD Succeed With Login
    }
    else
    {
        qDebug() << QString::fromLocal8Bit("MD行情登录失败"); //MD Login Failed
    }
}

void CtpMdApi::OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (pRspInfo->ErrorID == 0)
    {
        // 退出标记
        is_md_logout.store(true);
        // 登出成功
        qDebug() << QString::fromLocal8Bit("MD成功退出");//MD  Succeed With  Logout
    }
    else
    {

    }
}

void CtpMdApi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    // 仅供UI定时刷新展示(Per 3 Seconds)
    QString id = pDepthMarketData->InstrumentID;
    if (de->lastMarketDataSet.find(id) != de->lastMarketDataSet.end())
    {
        //如果存在，则更新
        de->lastMarketDataSet[id] = *pDepthMarketData;
    }
    else
    {
        de->lastMarketDataSet.insert(id, *pDepthMarketData);
    }

    // 推送策略Tick事件
    Event tickEvent(EVENT_TICK);    //Event.type_ == EVENT_TICK
    tickEvent.data_ = std::move(std::shared_ptr<char>((char*)new QuoteInfo));  //将Tick报价指针转化为char*指针
    QuoteInfo* quote = (QuoteInfo*)tickEvent.data_.get();                      //将data_指针赋值给quote

    quote->symbol = (pDepthMarketData->InstrumentID);
    quote->vtSymbol = (quote->symbol);

    quote->upperLimit = (pDepthMarketData->UpperLimitPrice);
    quote->lowerLimit = (pDepthMarketData->LowerLimitPrice);

    quote->openPrice = (pDepthMarketData->OpenPrice);
    quote->highPrice = (pDepthMarketData->HighestPrice);
    quote->lowPrice = (pDepthMarketData->LowestPrice);
    quote->lastPrice = (pDepthMarketData->LastPrice);

    quote->bidPrice1 = (pDepthMarketData->BidPrice1);
    quote->bidVolume1 = (pDepthMarketData->BidVolume1);
    quote->askPrice1 = (pDepthMarketData->AskPrice1);
    quote->askVolume1 = (pDepthMarketData->AskVolume1);

    quote->volume = (pDepthMarketData->Volume);
    quote->openInterest = (pDepthMarketData->OpenInterest);
    quote->preClosePrice = (pDepthMarketData->PreClosePrice);

    // quote->setTime(QString(QLatin1String(pDepthMarketData->UpdateTime) + "%1%2").arg(":").arg(QString::number(pDepthMarketData->UpdateMillisec)));
    quote->time = (pDepthMarketData->UpdateTime);
    quote->tradingDay = (pDepthMarketData->TradingDay);

    ee->putEvent(tickEvent);     //事件引擎__queue 中加入tickEvent

    //qDebug() << "contractName: " << pDepthMarketData->InstrumentID << "lastPrice: " << pDepthMarketData->LastPrice << "updateTime: " << pDepthMarketData->UpdateTime;

}
