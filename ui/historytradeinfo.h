#ifndef HISTORYTRADE_H
#define HISTORYTRADE_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"

class HistoryTrade:public BaseView
{
public:
    HistoryTrade(QWidget *parent,QString s);
    ~HistoryTrade();

public slots:
    //更新表格
    void updateData(Event ev);

private:
    void initData(Event ev);
    void refrushTable(QList<TradeInfo> &finishedOrder);
    QList<TradeInfo> grid_data_list;
};

#endif // HISTORYTRADE_H
