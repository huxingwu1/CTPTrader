#ifndef WEITUOINFO_H
#define WEITUOINFO_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"

/*
 * WeituoInfo只包含一个QDockWidget,2个QTableWidget,2个headerName
 */

class WeituoInfo: public BaseView
{
public:
    WeituoInfo(QWidget *parent,QString s);
    ~WeituoInfo();

public slots:
    //更新表格
    void updateData(Event ev);

private:
    void initData(Event ev);
    void refrushTable(QList<OrderInfo> &all_order);
    QList<OrderInfo> grid_data_list;
};

#endif // WEITUOINFO_H
