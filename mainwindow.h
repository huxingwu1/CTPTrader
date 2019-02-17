#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopWidget>
#include <QApplication>
#include <QMenuBar>    //菜单栏
#include <QToolBar>    //工具栏
#include <QStatusBar>  //状态栏
#include <QLabel>
#include <QGroupBox>
#include <QTableWidget>
#include <QStringList>
#include <QTabWidget>
#include <QDockWidget>
#include <QString>
#include <QPixmap>
#include <QPalette>
#include <QRect>
#include <QByteArray>
#include <QMutex>
#include <QMessageBox>
#include <QThread>
#include <QPoint>
#include <QMenu>
#include "login.h"
#include "myemail.h"
#include "datafromlocal.h"
#include "login_otheraccount.h"
#include "searchinstrument.h"
#include "ui/accountinfo.h"
#include "ui/hangqinginfo.h"
#include "ui/weituoinfo.h"
#include "ui/historytradeinfo.h"
#include "ui/chicanginfo.h"
#include "ui/tradelog.h"
#include "ui/tradeboardinfo.h"
#include "ui/instrumentinfo.h"
#include "ui/strategymonitor.h"
#include "mainengine.h"

extern MainEngine* me;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //事件过滤
    bool eventFilter(QObject *target, QEvent *e) override;

public slots:
    //显示登录界面框
    void showlogin();
    // 添加多账户界面
    void showlogin_n();
    // 登录函数
    void MDTDLogin(QString);
    // 添加多账户登录
    void MDTDLogin_n(QString);

    void OnInstrmentLineTextSubscribe();
    void OnSendOrderClicked();
    void OnCancelAll();
    void OnCancelOne();    //当前合约表选中右键撤单
    //持仓表选中右键平仓
    void OffsetOne();

    void OnStartCTA();
    void OnStopCTA();
    //委托表 右键撤单
    void OnWtMenu(const QPoint& pt);
    void OnWtMenu1(const QPoint& pt);
    //打印日志
    void PrintLog(QString s);

private:
    //界面布局初始化
    void viewInit();
    //设置窗口
    void setWindow(int x, int y,int w,int h);
    //设置菜单栏
    void setMenu();
    //设置工具栏
    void setTool();
    //设置状态栏
    void setStatus();
    //恢复界面初始化状态
    void restoreView();
    //不用new和delete，一种更安全的方式
    void restoreView1();
    // 获得订单数据
    bool getSendOrderRequest(orderCommonRequest& order_field);

public:
    TradeLog *tradelog;
    TradeboardInfo *tradeboard;   //"hangqinginfo.cpp"调用
private:
    Login * mylogin {nullptr};   //登陆框指针
    Login_otheraccount *mylogin_n;  //添加多账户
    MyEmail * myemail {nullptr};      //邮件
    SearchInstrument *mysearch  {nullptr};   //查询合约
    DataFromLocal * datalocal;      //添加本地数据

    Ui::MainWindow *ui;
    MyAccountInfo *account;
    HangqingInfo *hangqing;
    WeituoInfo *weituo;
    HistoryTrade * historytrade;
    ChicangInfo *chicang;
    //TradeLog *tradelog;
    //TradeboardInfo *tradeboard;
    InstrumentShow *instrument;
    StrategyMonitor *strategymonitor;

    //右键撤单
    QAction *actioncd;
    //右键平仓
    QAction *actionOffset;

    // 索引编辑框
    //CompleteLineEdit* order_contract_edit;

};

#endif // MAINWINDOW_H
