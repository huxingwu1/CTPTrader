#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QObject>
#include <QDebug>
#include <QWidget>
#include <QDockWidget>
#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QHeaderView>
#include <QPixmap>
#include <QPalette>
#include <QTableWidgetItem>
#include <QColor>
#include <QAction>
#include <QHeaderView>

/*
 * 显示框的基类，包含一个QDockWidget,QTableWidget,headerName
 */

class BaseView : public QWidget
{
    Q_OBJECT
public:
    //explicit BaseView(QObject *parent = 0);
    BaseView(QWidget *parent,QString s)
    {
        Dock = new QDockWidget(s,parent);
        firstTable = new QTableWidget(parent);
    }

    //QDockWidget初始化
    void DockInit(int x,int y,int w,int h);

    //将第一张表加入QDockWidget
    void addTable(int w,int h,QTableWidget *Table, QStringList headerName);

signals:

public:
    QDockWidget *Dock;
    QTableWidget *firstTable;
    QStringList headerName;

public slots:
};

#endif // BASEVIEW_H
