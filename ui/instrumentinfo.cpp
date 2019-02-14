#include "instrumentinfo.h"
#include "mainengine.h"

extern MainEngine *me;

InstrumentShow::InstrumentShow(QWidget *parent, QString s):BaseView(parent,s)
{
    Dock->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->DockInit(0,0,900,200);
    headerName.empty();
    headerName.append(QString::fromLocal8Bit("合约代码"));
    headerName.append(QString::fromLocal8Bit("名称"));
    headerName.append(QString::fromLocal8Bit("交易所"));
    headerName.append(QString::fromLocal8Bit("最后交割日"));
    headerName.append(QString::fromLocal8Bit("保证金率"));
    headerName.append(QString::fromLocal8Bit("合约乘数"));
    headerName.append(QString::fromLocal8Bit("开仓手续费"));
    headerName.append(QString::fromLocal8Bit("平仓手续费"));
    headerName.append(QString::fromLocal8Bit("平今手续费"));
    headerName.append(QString::fromLocal8Bit("最小变动单位"));
    headerName.append(QString::fromLocal8Bit("是否可以交易"));
    this->addTable(900,200,firstTable,headerName);

    // 更新合约窗口事件
    me->register_event(EVENT_CONTRACT, this, &InstrumentShow::updateData);
}


InstrumentShow::~InstrumentShow()
{

}

void InstrumentShow::updateData(Event ev)
{
    qDebug()<<"InstrumentShow::updateData";      //显示合约信息影响性能，暂时不显示
    initData(ev);
    refrushTable(grid_data_list);
    saveInstrument(grid_data_list);
}

void InstrumentShow::initData(Event ev)
{
    grid_data_list.clear();

    QMap <QString, InstrumentInfo>& all_contracts = me->me_getInstrumentInfo();

    QMap <QString, InstrumentInfo>::iterator it;
    for (it = all_contracts.begin(); it != all_contracts.end(); ++it)
    {
        InstrumentInfo qm;
        qm.id = (it->id);
        qm.name = (it->name);
        qm.exchangeId = (it->exchangeId);
        qm.deadline = (it->deadline);
        qm.marginRate = (it->marginRate);
        qm.multiplier = (it->multiplier);
        qm.openCommission = (it->openCommission);
        qm.closeCommission = (it->closeCommission);
        qm.closeTodayCommission = (it->closeTodayCommission);
        qm.minimumUnit = (it->minimumUnit);
        qm.tradable = (it->tradable);

        grid_data_list.append(qm);
    }
}


void InstrumentShow::refrushTable(QList<InstrumentInfo> &all_instrument)
{
    //清空持仓表
    for(int i=firstTable->rowCount();i>=0;i--)
    {
        firstTable->removeRow(i);
    }
    //将持仓写入表中
    for(int row = 0; row< all_instrument.size(); row++)
    {
        firstTable->insertRow(row);
        firstTable->setItem(row, 0, new QTableWidgetItem(all_instrument.at(row).id));
        firstTable->setItem(row, 1, new QTableWidgetItem(all_instrument.at(row).name));
        firstTable->setItem(row, 2, new QTableWidgetItem(all_instrument.at(row).exchangeId));
        firstTable->setItem(row, 3, new QTableWidgetItem(all_instrument.at(row).deadline.toString("yyyy.MM.dd")));
        firstTable->setItem(row, 4, new QTableWidgetItem(QString::number(all_instrument.at(row).marginRate)));
        firstTable->setItem(row, 5, new QTableWidgetItem(QString::number(all_instrument.at(row).multiplier)));
        firstTable->setItem(row, 6, new QTableWidgetItem(QString::number(all_instrument.at(row).openCommission)));
        firstTable->setItem(row, 7, new QTableWidgetItem(QString::number(all_instrument.at(row).closeCommission)));
        firstTable->setItem(row, 8, new QTableWidgetItem(QString::number(all_instrument.at(row).closeTodayCommission)));
        firstTable->setItem(row, 9, new QTableWidgetItem(QString::number(all_instrument.at(row).minimumUnit)));
        firstTable->setItem(row, 10, new QTableWidgetItem(QString::number(all_instrument.at(row).tradable)));


    }
}

void InstrumentShow::saveInstrument(QList<InstrumentInfo> &rowlist)
{

}
