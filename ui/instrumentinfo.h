#ifndef INSTRUMENTINFO_H
#define INSTRUMENTINFO_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"

// 展示合约信息
class InstrumentShow : public BaseView
{
public:
    InstrumentShow(QWidget *parent,QString s);
    ~InstrumentShow();

public slots:
    //更新表格
    void updateData(Event ev);

private:
    void initData(Event ev);
    void refrushTable(QList<InstrumentInfo> &rowlist);
    //保存合约信息到本地文件
    void saveInstrument(QList<InstrumentInfo> &rowlist);

    QList<InstrumentInfo> grid_data_list;
};



#endif // INSTRUMENTINFO_H
