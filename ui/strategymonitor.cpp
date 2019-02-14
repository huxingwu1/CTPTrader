#include "strategymonitor.h"

StrategyMonitor::StrategyMonitor(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::TopDockWidgetArea);
    this->DockInit(0,0,900,200);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("策略名称"));
    this->addTable(900,200,firstTable,headerName);
}

StrategyMonitor::~StrategyMonitor()
{

}
