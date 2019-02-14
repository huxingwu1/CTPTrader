#ifndef CHICANGINFO_H
#define CHICANGINFO_H

#include "baseview.h"
#include "publicstruct.h"
#include "eventbase.h"
#include <QAction>
#include <QMenu>
#include <QPoint>
#include <QThread>

class ChicangInfo:public BaseView
{
public:
    ChicangInfo(QWidget *parent,QString s);
    ~ChicangInfo();

public slots:
    //更新表格
    void updateData(Event ev);

private:
    void initData(Event ev);
    void refrushTable(QList<PositionInfo>& all_position);
    QList<PositionInfo> grid_data_list;

};

#endif // CHICANGINFO_H
