#ifndef HANGQINGINFO_H
#define HANGQINGINFO_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"
#include "ThostFtdcUserApiStruct.h"
#include <QTimer>

/*
 * HangqingInfo只包含一个QDockWidget,QTableWidget,headerName
 */


class MainWindow;
class HangqingInfo: public BaseView
{
public:
    HangqingInfo(QWidget *parent,QString s);
    ~HangqingInfo();

public slots:
    //更新表格
    void updateData();

private:
    void initData();
    void refrushTable(QList<QuoteInfo>& all_position);

    QList<QuoteInfo> grid_data_list;
    //更新table
    QTimer *updateTimer;

};

#endif // HANGQINGINFO_H
