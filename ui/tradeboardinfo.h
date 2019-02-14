#ifndef TRADEBOARDINFO_H
#define TRADEBOARDINFO_H

#include <QObject>
#include <QWidget>
#include <QDockWidget>
#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QHeaderView>
#include <QPixmap>
#include <QPalette>
#include <QPushButton>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include <QRegExp>
#include <QValidator>


class TradeboardInfo
{
public:
    TradeboardInfo(QWidget *parent,QString s)
    {
        Dock =new QDockWidget(s,parent);
        Dock->setAllowedAreas(Qt::TopDockWidgetArea);
        this->DockInit(900,0,300,400);

        this->tradeboardInit(parent);

    }
    //QDockWidget初始化
    void DockInit(int x,int y,int w,int h);
    //tradeboard下单面板布局
    void tradeboardInit(QWidget *parent);

public:
    QDockWidget *Dock;

    struct tradeboardnumber{
        QLabel *InstrumentID;
        QLineEdit *Instrument;
        QLabel *UpperLimitPrice;
        QLabel *UpperLimitPrice_show;  //显示数据的QLabel

        QLabel *vtName;
        QLineEdit *vtName_linedit;
        QLabel *AskPrice1_vol;          //卖1量
        QLabel *AskPrice1_vol_show;

        QLabel *openClose;
        QComboBox *openClose_comboBox;
        QLabel *AskPrice1;          //卖1价格
        QLabel *AskPrice1_show;

        QLabel *direction;
        QComboBox *direction_comboBox;
        QLabel *newPrice;          //最新价格
        QLabel *newPrice1_show;

        QLabel *price;
        QDoubleSpinBox *price_spinbox;
        QLabel *BidPrice1;          //买1价格
        QLabel *BidPrice1_show;

        QLabel *pricetype;
        QComboBox *pricetype_comboBox;
        QLabel *BidPrice1_vol;          //买1量
        QLabel *BidPrice1_vol_show;

        QLabel *number;
        QSpinBox *number_spinbox;
        QLabel *LowerLimitPrice;          //跌停价格
        QLabel *LowerLimitPrice_show;


        QPushButton * xiadan;    //下单按钮
        QPushButton * cancel;    //全撤按钮
    }tradeboard;

};

#endif // TRADEBOARDINFO_H
