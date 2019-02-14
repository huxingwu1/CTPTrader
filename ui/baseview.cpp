#include "baseview.h"


void BaseView::DockInit(int x, int y, int w, int h)
{
    Dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    //停靠区域放在外面设置
    //Dock->setAllowedAreas(Qt::LeftDockWidgetArea |Qt::TopDockWidgetArea |Qt::RightDockWidgetArea |Qt::BottomDockWidgetArea);//Qt::TopDockWidgetArea |Qt::LeftDockWidgetArea |Qt::RightDockWidgetArea

    //添加背景图片，设置背景颜色
    Dock->setAutoFillBackground(true);
    Dock->setStyleSheet("background-color: rgb( 45, 45, 45);color: white;};");

//    QPixmap pixmap(":/icon/formskin.png");//设定图片
//    QPalette palette;//创建一个调色板对象
//    palette.setBrush(QPalette::Window,QBrush(pixmap));//用调色板的画笔把映射到pixmap上的图片画到frame.backgroundRole()这个背景上
//    palette.setColor(QPalette::Background, Qt::gray);
//    Dock->setPalette(palette);

    //Dock->setWidget(firstTable);    //将firstTable添加进入Dock
    Dock->setGeometry(x ,y ,w ,h);
    Dock->setMinimumSize(w ,h);
    Dock->setMaximumSize(w ,h+300);    //框悬浮时可以下拉200

}

void BaseView::addTable(int w,int h,QTableWidget *Table, QStringList headerName)
{
    this->Dock->setWidget(Table);
    Table->setParent(this->Dock);
    Table->setFrameShape(QFrame::NoFrame);//设置边框
    //Table->setShowGrid(false); //设置不显示格子线

    //Table->setStyleSheet("QHeaderView::section{background-color:rgb(0,20,30);font:13pt '宋体';color: white;};");
    Table->setStyleSheet("QHeaderView::section{background-color:rgb( 69, 137, 148); color: white;};");


    //表格设置背景颜色
//    QPalette pll = Table->palette();
//    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
//    Table->setPalette(pll);

    Table->setGeometry(0,20,w,h-20);   //Dock表头的宽度
    Table->setMinimumSize(w,h-20);
    Table->setMaximumSize(w,h+280);
    //Table->item(1,1)->setBackgroundColor( Qt::red);
    //Table->item(0,1)->setTextColor(QColor(200,111,30)); //设置文字颜色
    //Table->setItem(1,1,);
//    QTableWidgetItem *item = Table->item(0,0);
//    const QColor color = QColor(252,222,156);
//    item->setBackgroundColor(color);

    //账户表
//    Table->setAutoFillBackground(true);
//    QPixmap pixmap(":/icon/formskin.png");//设定图片
//    QPalette palette;//创建一个调色板对象
//    palette.setBrush(QPalette::Window,QBrush(pixmap));//用调色板的画笔把映射到pixmap上的图片画到frame.backgroundRole()这个背景上
//    //palette.setColor(QPalette::Background, Qt::gray);
//    Table->setPalette(palette);

    Table->setColumnCount(headerName.size());
    Table->setHorizontalHeaderLabels(headerName);
    //Table->setShowGrid(false);                                 //隐藏表格线
    Table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//Stretch ResizeToContents);  //将表头显示在表内
    Table->setSelectionBehavior(QAbstractItemView::SelectRows);             //选中一行
    Table->setEditTriggers(QAbstractItemView::NoEditTriggers);              //表格不可编辑
    Table->verticalHeader()->setHidden(true);                               // 隐藏行号
}

