#ifndef TRADELOG_H
#define TRADELOG_H

#include "baseview.h"

class TradeLog:public BaseView
{
public:
    TradeLog(QWidget *parent,QString s):BaseView(parent,s)
    {
        Dock->setAllowedAreas(Qt::BottomDockWidgetArea);
        this->DockInit(0,0,600,180);
        headerName.empty();
        headerName.append(QString::fromLocal8Bit("类型"));
        headerName.append(QString::fromLocal8Bit("时间"));
        headerName.append(QString::fromLocal8Bit("内容"));
        this->addTable(600,180,firstTable,headerName);

    }
};

#endif // TRADELOG_H
