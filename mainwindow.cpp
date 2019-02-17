#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->close();     //关闭mainwindow默认的工具栏,后期ui界面移除mainToolBar
    setCentralWidget(0);
//    QSpinBox{ height:30px; width:30px;}
//    QSpinBox::up-button{ background-image:url(:/images/scroll_up.png); height: 15px;}QSpinBox::down-button{    background-image:url(:/images/scroll_down.png); height: 15px;}

    //setStyleSheet("QDoubleSpinBox::down-button{subcontrol-origin:border;subcontrol-position:left;border-image: url(:/icon/downarrow.png);width: 12px;height: 20px;");
    setStyleSheet("font:11pt '宋体';};");    //字体大小颜色

    //将mainwindow设置在正中间，长/宽（1200/680）
    QDesktopWidget * deskTop = QApplication::desktop();
    int curMonitor = deskTop->screenNumber ( this );
    const QRect rect = deskTop->screenGeometry(curMonitor);
    setWindow((rect.width()-1200)/2,(rect.height()-680)/2,1200,680);     //y=660接近状态栏

    //mainwindow添加背景颜色
    setAutoFillBackground(true);
    setStyleSheet("background-color: rgb( 69,69,69); color: white; ");

    //设置菜单栏
    setMenu();

    //设置工具栏
    //setTool();

    //设置状态栏
    setStatus();

    //界面布局初始化
    viewInit();

    mylogin = new Login(this);            //登录   //对于自定义类型，都会调用默认构造函数，加不加括号没区别。
    mylogin_n = new Login_otheraccount;   //多账户登录
    myemail = new MyEmail;                //邮件
    mysearch = new SearchInstrument;      //  查询合约
    datalocal = new DataFromLocal;        //添加本地数据
    actioncd = new QAction(QString::fromLocal8Bit("撤单"), this);
    actionOffset = new QAction(QString::fromLocal8Bit("平仓"), this);
    //connect连接
    connect(this->mylogin, SIGNAL(sendAccount(QString)), this, SLOT(MDTDLogin(QString)));
    connect(actioncd, SIGNAL(triggered()), this, SLOT(OnCancelOne()));   //右键撤单
    connect(actionOffset, SIGNAL(triggered()), this, SLOT(OffsetOne()));   //右键平仓
    connect(me,SIGNAL(sendLog(QString)),this,SLOT(PrintLog(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
    //delete account;
}

// 这是事件过滤器似乎没有额外的作用，看上去使用信号槽相应登录/退出更优
bool MainWindow::eventFilter(QObject *target, QEvent *e)
{
//    if (target == ui.pushButton_login)
//    {
//        if (e->type() == QEvent::MouseButtonRelease)
//        {
//            QString userid = ui.lineEdit_userID->text();
//            QString psd = ui.lineEdit_password->text();
//            if (userid.isEmpty() || psd.isEmpty())
//            {
//                return false;
//            }

//            ui.pushButton_login->setEnabled(false);
//            ui.pushButton_logout->setEnabled(true);

//            ui.lineEdit_userID->setEnabled(false);
//            ui.lineEdit_password->setEnabled(false);

//            me->me_login(userid, psd, "9999", "tcp://180.168.146.187:10010", "tcp://180.168.146.187:10000");
//            //me->me_login(userid, psd, "9999", "tcp://180.168.146.187:10031", "tcp://180.168.146.187:10030");  //7*24
//            std::this_thread::sleep_for(std::chrono::milliseconds(300));

//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    else if (target == ui.pushButton_logout)
//    {
//        if (e->type() == QEvent::MouseButtonRelease)
//        {
//            ui.pushButton_login->setEnabled(true);
//            ui.pushButton_logout->setEnabled(false);

//            ui.lineEdit_userID->setEnabled(true);
//            ui.lineEdit_password->setEnabled(true);

//            me->me_logout();
//            std::this_thread::sleep_for(std::chrono::milliseconds(300));

//            // 关闭程序
//            close();

//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }

//    return QMainWindow::eventFilter(target, e);
    return true;
}

void MainWindow::showlogin()
{
    // 主登陆框多次登录相同账号 会导致程序崩溃，因此设置主登陆框只显示一次
    if(me->me_get_is_login())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("登录异常"),QString::fromLocal8Bit("程序已登录，重新登录请重启程序，添加账号请点击 “登录——添加多账号” "),QMessageBox::Ok);
    }
    else
    {
        mylogin->show();
    }

    //this->restoreState(saveMyState);
}

void MainWindow::showlogin_n()
{
    mylogin_n->show();
}

void MainWindow::MDTDLogin(QString account)
{
    //传过来的类型 md + "," + td + "," +brokerid +","+account +","+password;
    QStringList accountlist = account.split(",");
    QString md = accountlist.at(0);
    QString td = accountlist.at(1);
    QString broker = accountlist.at(2);
    QString userid = accountlist.at(3);
    QString password = accountlist.at(4);
    //void MainEngine::me_login(QString userid, QString password, QString brokerid, QString mdAddress, QString tdAddress);
    me->me_login(userid, password, broker, md, td);
    QThread::msleep(1000);     //暂停2秒
    qDebug()<<"MainWindow::MDTDLogin";

}

void MainWindow::MDTDLogin_n(QString account)
{
    //传过来的类型 td + "," +brokerid +","+account +","+password;
    QStringList accountlist = account.split(",");
    QString td = accountlist.at(0);
    QString broker = accountlist.at(1);
    QString userid = accountlist.at(2);
    QString password = accountlist.at(3);
    me->me_login(userid, password, broker, td);
    QThread::msleep(1000);     //暂停2秒
    qDebug()<<"MainWindow::MDTDLogin_n";
}

// 订阅行情
void MainWindow::OnInstrmentLineTextSubscribe()
{
    if (me->me_get_is_login())
    {
        me->me_subscribe(tradeboard->tradeboard.Instrument->text());
    }
}

void MainWindow::OnSendOrderClicked()
{
    // 下单: 默认手动下单为限价单，其他单（包括市价、中金所高级市价、大商所止盈止损以及FAK/FOK等只在策略中调用）
    // 按道理，每发一笔单就得在本地维护一个委托单列表
    if (me->me_get_is_login())
    {
        orderCommonRequest order_field;
        if (getSendOrderRequest(order_field))
        {
            me->me_sendDefaultOrder(order_field);
        }
        else
        {
            int ret = QMessageBox::warning(this, QString::fromLocal8Bit("数据异常"),QString::fromLocal8Bit("下单数据异常，请检查后重新下单"),QMessageBox::Ok | QMessageBox::Cancel);
        }
    }
    else
    {
        qDebug()<<QString::fromLocal8Bit("ma td 暂未登录");
    }
}

// 一键全撤
void MainWindow::OnCancelAll()
{
    if (me->me_get_is_login())
    {
        QMap<QString, OrderInfo>&& working_orders = me->me_getWorkingOrderInfo();
        for (auto it = working_orders.begin(); it != working_orders.end(); ++it)
        {
            cancelCommonRequest cancel_field;
            strncpy_s(cancel_field.instrument, it->symbol.toStdString().c_str(), sizeof(cancel_field.instrument));
            strncpy_s(cancel_field.exchange, it->exchange.toStdString().c_str(), sizeof(cancel_field.exchange));
            strncpy_s(cancel_field.order_ref, it->orderID.toStdString().c_str(), sizeof(cancel_field.order_ref));
            cancel_field.front_id = it->frontID;
            cancel_field.session_id = it->sessionID;
            me->me_cancelOrder(cancel_field);
            //std::this_thread::sleep_for(std::chrono::milliseconds(1100));
        }
    }
}

// 委托表右键撤单
void MainWindow::OnCancelOne()
{
    int i = weituo->firstTable->currentIndex().row();         //委托表所在的行数
    if(-1 ==i)
    {
        qDebug()<<QString::fromLocal8Bit("委托表格为空.");
    }
    else
    {
        int total = weituo->firstTable->item(i, 6)->text().toInt();
        int trade = weituo->firstTable->item(i, 7)->text().toInt();
        QString status = weituo->firstTable->item(i, 8)->text();
        if(total == trade || status ==QString::fromLocal8Bit("已撤单"))
        {
            int ret = QMessageBox::warning(this, QString::fromLocal8Bit("撤单失败"),QString::fromLocal8Bit("未成交数量为0,或已撤单,请确认!"),QMessageBox::Ok);
        }
        else
        {
            //从委托表所在行获得撤单的信息
            cancelCommonRequest cancel_field;
            strncpy_s(cancel_field.order_ref, weituo->firstTable->item(i, 0)->text().toStdString().c_str(), sizeof(cancel_field.order_ref));   //委托编号
            strncpy_s(cancel_field.instrument, weituo->firstTable->item(i, 1)->text().toStdString().c_str(), sizeof(cancel_field.instrument));
            strncpy_s(cancel_field.exchange,  weituo->firstTable->item(i, 2)->text().toStdString().c_str(), sizeof(cancel_field.exchange));

            cancel_field.front_id = weituo->firstTable->item(i, 11)->text().toInt();
            cancel_field.session_id = weituo->firstTable->item(i, 12)->text().toInt();
            me->me_cancelOrder(cancel_field);
        }
    }
}

//持仓表右键平仓
//如果不是上期所，平今仓可用close或closeToday，平昨仓可用close或closeYesterday。
//如果是上期所，   平今仓只可用closeToday，平昨仓可用close或closeYesterday。
//默认按最新价成交，可以按对手价成交。
void MainWindow::OffsetOne()
{
    int i = chicang->firstTable->currentIndex().row();
    if(-1 ==i)
    {
        qDebug()<<QString::fromLocal8Bit("持仓表格为空.");
    }
    else
    {
        qDebug()<<"MainWindow::OffsetOne()";
        int tdPoistion = chicang->firstTable->item(i, 4)->text().toInt();   //今持仓
        int ydPoistion = chicang->firstTable->item(i, 5)->text().toInt();   //昨持仓
        QString instrument =  chicang->firstTable->item(i, 0)->text();      //代码
        MainEngine::AskBidPrice askbid = me->me_getInstrumentPrice(instrument);
        if(0 == tdPoistion && 0 == ydPoistion)
        {
            int ret = QMessageBox::warning(this, QString::fromLocal8Bit("平仓失败"),QString::fromLocal8Bit("可平数量为0,请重新确认!"),QMessageBox::Ok);
        }
        if(tdPoistion >0)
        {
            //从持仓表所在行获得平单的信息
            orderCommonRequest offset_field;
            // 代码
            strncpy_s(offset_field.instrument, instrument.toStdString().c_str(), sizeof(offset_field.instrument) - 1);
            // 价格
            offset_field.price = askbid.new1;
            // 数量
            offset_field.volume = tdPoistion; //QString::fromLocal8Bit("买多")
            if(QString::fromLocal8Bit("买 多") == chicang->firstTable->item(i, 2)->text())
            {
                offset_field.direction = THOST_FTDC_D_Sell;
            }
            else if(QString::fromLocal8Bit("卖 空") == chicang->firstTable->item(i, 2)->text())
            {
                offset_field.direction = THOST_FTDC_D_Buy;
            }
            else
            {
                qDebug()<<"MainWindow::OffsetOne tdPoistion direction uncertain";
            }
            //平今 THOST_FTDC_OF_CloseToday 对所有交易所都有用
            offset_field.offset = THOST_FTDC_OF_CloseToday;  //THOST_FTDC_OF_Close   THOST_FTDC_OF_CloseToday
            me->me_sendDefaultOrder(offset_field);
        }
        if(ydPoistion >0)
        {
            //从持仓表所在行获得平单的信息
            orderCommonRequest offset_field;
            // 代码
            strncpy_s(offset_field.instrument, instrument.toStdString().c_str(), sizeof(offset_field.instrument) - 1);
            // 价格
            offset_field.price = askbid.new1;
            // 数量
            offset_field.volume = tdPoistion;//QString::fromLocal8Bit("买多")
            if(QString::fromLocal8Bit("买 多") == chicang->firstTable->item(i, 2)->text())
            {
                offset_field.direction = THOST_FTDC_D_Sell;
            }
            else if(QString::fromLocal8Bit("卖 空") == chicang->firstTable->item(i, 2)->text())
            {
                offset_field.direction = THOST_FTDC_D_Buy;
            }
            else
            {
                qDebug()<<"MainWindow::OffsetOne ydPoistion direction uncertain";
            }
            offset_field.offset = THOST_FTDC_OF_CloseYesterday;
            me->me_sendDefaultOrder(offset_field);
        }
    }
}

void MainWindow::OnStartCTA()
{
    if (me->me_get_is_login())
    {
        me->me_strat_cta();
    }
}

void MainWindow::OnStopCTA()
{
    if (me->me_get_is_login())
    {
        me->me_stop_cta();
    }
}

void MainWindow::OnWtMenu(const QPoint &pt)
{
    QMenu menu;
    menu.addAction(actioncd);
    menu.exec(this->weituo->firstTable->mapToGlobal(pt));
}

void MainWindow::OnWtMenu1(const QPoint &pt)
{
    QMenu menu1;
    menu1.addAction(actionOffset);
    menu1.exec(this->chicang->firstTable->mapToGlobal(pt));
}

void MainWindow::PrintLog(QString s)
{
    QStringList strlist = s.split("++");                                
    if("Debug:"== strlist.at(0))   // 目前只当前只打印 Debug
    {
        int row = tradelog->firstTable->rowCount();
        tradelog->firstTable->insertRow(row);
        tradelog->firstTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
        tradelog->firstTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
        tradelog->firstTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));

    }
}

void MainWindow::viewInit()
{
    hangqing = new HangqingInfo(this,QString::fromLocal8Bit("行情"));
    weituo= new WeituoInfo(this,QString::fromLocal8Bit("当前委托"));
    //委托表  右键
    weituo->firstTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(weituo->firstTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWtMenu(const QPoint&)));

    historytrade = new HistoryTrade(this,QString::fromLocal8Bit("历史成交"));
    instrument = new InstrumentShow(this,QString::fromLocal8Bit("合约信息"));
    account = new MyAccountInfo(this,QString::fromLocal8Bit("资金账户"));
    chicang = new ChicangInfo(this,QString::fromLocal8Bit("当前持仓"));
    chicang->firstTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chicang->firstTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWtMenu1(const QPoint&)));

    tradelog = new TradeLog(this,QString::fromLocal8Bit("交易日志"));
    tradeboard = new TradeboardInfo(this,QString::fromLocal8Bit("交易"));
    strategymonitor = new StrategyMonitor(this,QString::fromLocal8Bit("策略监控"));
    //代码框 回车按钮槽函数
    connect(tradeboard->tradeboard.Instrument,&QLineEdit::returnPressed,[=](){
        qDebug()<<"OnInstrmentLineTextSubscribe()";
        OnInstrmentLineTextSubscribe();
    });
    //下单按钮槽函数
    connect(tradeboard->tradeboard.xiadan,&QPushButton::clicked,[=](){
        OnSendOrderClicked();
    });
    //一键全撤
    connect(tradeboard->tradeboard.cancel,&QPushButton::clicked,[=](){
        OnCancelAll();
    });


    addDockWidget(Qt::LeftDockWidgetArea, hangqing->Dock);
    addDockWidget(Qt::RightDockWidgetArea, tradeboard->Dock);
    addDockWidget(Qt::LeftDockWidgetArea, weituo->Dock);
    addDockWidget(Qt::LeftDockWidgetArea, historytrade->Dock);
    addDockWidget(Qt::LeftDockWidgetArea, instrument->Dock);
    addDockWidget(Qt::LeftDockWidgetArea, strategymonitor->Dock);
    addDockWidget(Qt::BottomDockWidgetArea, account->Dock);
    addDockWidget(Qt::BottomDockWidgetArea, chicang->Dock);
    addDockWidget(Qt::BottomDockWidgetArea, tradelog->Dock);
    //将委托表和历史成交表合并
    this->tabifyDockWidget(weituo->Dock,historytrade->Dock);
    this->tabifyDockWidget(weituo->Dock,strategymonitor->Dock);
    this->tabifyDockWidget(weituo->Dock,instrument->Dock);
    this->tabifyDockWidget(account->Dock,chicang->Dock);
    weituo->Dock->raise();
    account->Dock->raise();
    hangqing->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    weituo->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    historytrade->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    instrument->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    account->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    chicang->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    tradelog->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");
    strategymonitor->Dock->setStyleSheet("background-color: rgb( 245, 245, 145); color: black;");

//    QPushButton * qw=new QPushButton(this);
//    //qw->setStyleSheet("background-color: rgb( 45, 45, 45);");
//    qw->setStyleSheet(tr("background-color:red;"));
//    weituo->Dock->setTitleBarWidget(qw);
//    //historytrade->Dock->setTitleBarWidget(qw);


//    splitDockWidget(account->Dock,chicang->Dock,Qt::Vertical);   //排列DockWidget
//    this->tabifyDockWidget(account->Dock,chicang->Dock);     //将两个dockWidget窗口组成一个Widget stack,默认显示的是第2个窗口
//    this->tabifyDockWidget(account->Dock,tradelog->Dock);
//    account->Dock->raise();
}

void MainWindow::setWindow(int x, int y, int w, int h)
{
    setWindowTitle("CTPTrader");
    setWindowIcon(QIcon(":/icon/qt.png"));
    setGeometry(x,y,w,h);
    setMinimumSize(w,h);
    setMaximumSize(w+200,h+100);
}

//设置菜单栏
void MainWindow::setMenu()
{
    QMenuBar *mb = ui->menuBar;//this->menuBar();
    QMenu *menuLogin = mb->addMenu(QString::fromLocal8Bit("登录(L)"));
    QMenu *menuBacktest = mb->addMenu(QString::fromLocal8Bit("回测(B)"));
    QMenu *menuStrategy = mb->addMenu(QString::fromLocal8Bit("策略(S)"));
    QMenu *menuData = mb->addMenu(QString::fromLocal8Bit("数据(D)"));
    QMenu *menuHelp = mb->addMenu(QString::fromLocal8Bit("帮助(H)"));

    //登录栏
    QAction *actionLogin = menuLogin->addAction(QIcon(":/icon/spqy.png"),QString::fromLocal8Bit("登录"));
    //登录按钮槽函数
    connect(actionLogin,&QAction::triggered,[=](){
        showlogin();
    });

    QAction *actionLogout = menuLogin->addAction(QIcon(":/icon/spqy.png"),QString::fromLocal8Bit("退出"));
    //退出按钮槽函数
    connect(actionLogout,&QAction::triggered,[=](){
        me->me_logout();
        QThread::msleep(1000);     //暂停1秒
        //关闭程序
        close();
    });

    menuLogin->addSeparator();      //添加分隔符
    QAction *actionLogin_n = menuLogin->addAction(QIcon(":/icon/spqy.png"),QString::fromLocal8Bit("添加多账户"));
    connect(actionLogin_n,&QAction::triggered,[=](){
        showlogin_n();
    });


    //回测栏
    QAction *actionBacktest = menuBacktest->addAction(QString::fromLocal8Bit("回测"));

    //策略栏
    QAction *actionLoadStrategy = menuStrategy->addAction(QIcon(":/icon/add.jpg"),QString::fromLocal8Bit("加载策略"));
    //开始策略槽函数
    connect(actionLoadStrategy,&QAction::triggered,[=](){
        OnStartCTA();
    });
    QAction *actionCancelStrategy = menuStrategy->addAction(QIcon(":/icon/minus.jpg"),QString::fromLocal8Bit("停止策略"));
    //停止策略槽函数
    connect(actionCancelStrategy,&QAction::triggered,[=](){
        OnStopCTA();
    });
    QAction *actionDeleteStrategy = menuStrategy->addAction(QIcon(":/icon/delete.jpg"),QString::fromLocal8Bit("删除策略"));
    //停止策略槽函数
    connect(actionDeleteStrategy,&QAction::triggered,[=](){
        //
    });
    QAction *actionShowStrategy = menuStrategy->addAction(QString::fromLocal8Bit("当前策略列表"));


    //数据栏
    QAction *actionLoadData = menuData->addAction(QIcon(":/icon/save.png"),QString::fromLocal8Bit("加载本地数据"));
    connect(actionLoadData,&QAction::triggered,[=](){
        datalocal->show();
    });
    QAction *actionLoadData1 = menuData->addAction(QString::fromLocal8Bit("下载数据"));
    QAction *showLoadData = menuData->addAction(QString::fromLocal8Bit("查看本地数据"));

    //帮助栏
    QAction *recoveryView = menuHelp->addAction(QString::fromLocal8Bit("恢复界面"));
    connect(recoveryView,&QAction::triggered,[=](){
        restoreView1();
    });

    QAction *addEmail = menuHelp->addAction(QIcon(":/icon/email.png"),QString::fromLocal8Bit("添加邮件"));
    connect(addEmail,&QAction::triggered,[=](){
        if(nullptr != myemail)  //myemail全局变量
        {
            myemail->show();
        }
        else
        {
            qDebug()<<QString::fromLocal8Bit("myemail 没有实例化");
        }
    });

    QAction *searchInstrument = menuHelp->addAction(QString::fromLocal8Bit("查询合约"));
    connect(searchInstrument,&QAction::triggered,[=](){
        mysearch->show();
    });
}

//工具栏暂时不用
void MainWindow::setTool()
{
//    LoginAct = new QAction(QIcon(":/icon/spqy.png"), QString::fromLocal8Bit("登录"), this);
//    QToolBar *toolbar = ui->mainToolBar;//new QToolBar(QString::fromLocal8Bit("工具栏"),this);//new QToolBar(this);   //用系统默认的工具栏
//    toolbar->setWindowTitle(QString::fromLocal8Bit("工具栏"));    //工具栏设置名字
//    this->addToolBar(Qt::TopToolBarArea ,toolbar);

//    toolbar->addAction(LoginAct);
//    toolbar->addSeparator();    //分割条

//    toolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
//    toolbar->setFloatable(false);
//    toolbar->setMovable(false);
}

void MainWindow::setStatus()
{
    QStatusBar *stBar = this->statusBar();
    QLabel *leftLabel = new QLabel(QString::fromLocal8Bit("左侧标签"),this);
    stBar->addWidget(leftLabel);
    QLabel *rightLabel = new QLabel(QString::fromLocal8Bit("右侧标签"),this);
    stBar->addPermanentWidget(rightLabel);
    QLabel *cpuNumber = new QLabel(QString::fromLocal8Bit("100%"),this);
    stBar->insertPermanentWidget(1,cpuNumber);
}

//恢复界面初始化状态
void MainWindow::restoreView()
{
    static QMutex mutex;
    mutex.lock();
    if(hangqing !=nullptr)
    {
        hangqing->Dock->close();
        delete hangqing;
        hangqing = nullptr;
    }
    if(weituo !=nullptr)
    {
        weituo->Dock->close();
        delete weituo;
        weituo = nullptr;
    }
    if(historytrade !=nullptr)
    {
        historytrade->Dock->close();
        delete historytrade;
        historytrade = nullptr;
    }
    if(account !=nullptr)
    {
        account->Dock->close();
        delete account;
        account = nullptr;
    }
    if(chicang !=nullptr)
    {
        chicang->Dock->close();
        delete chicang;
        chicang = nullptr;
    }
    if(tradelog !=nullptr)
    {
        tradelog->Dock->close();
        delete tradelog;
        tradelog = nullptr;
    }
    if(tradeboard !=nullptr)
    {
        tradeboard->Dock->close();
        delete tradeboard;
        tradeboard = nullptr;
    }
    viewInit();
    // 解锁
    mutex.unlock();
}

void MainWindow::restoreView1()
{
    hangqing->Dock->close();
    weituo->Dock->close();
    historytrade->Dock->close();
    instrument->Dock->close();
    account->Dock->close();
    chicang->Dock->close();
    tradelog->Dock->close();
    tradeboard->Dock->close();

    hangqing->Dock->showNormal();
    weituo->Dock->showNormal();
    historytrade->Dock->showNormal();
    instrument->Dock->showNormal();
    account->Dock->showNormal();
    chicang->Dock->showNormal();
    tradelog->Dock->showNormal();
    tradeboard->Dock->showNormal();

}

//tradeboard->tradeboard.
bool MainWindow::getSendOrderRequest(orderCommonRequest &order_field)
{
    bool get_ret = true;

    // instrument_id
    strncpy_s(order_field.instrument, tradeboard->tradeboard.Instrument->text().toStdString().c_str(), sizeof(order_field.instrument) - 1);
    // volume
    order_field.volume = tradeboard->tradeboard.number_spinbox->text().toInt();
    // price
    if (0 == tradeboard->tradeboard.pricetype_comboBox->currentIndex())  //限价
    {
        order_field.price = tradeboard->tradeboard.price_spinbox->text().toDouble();
    }
    else if(1 == tradeboard->tradeboard.pricetype_comboBox->currentIndex())   //市价按最新成交价
    {
        order_field.price = tradeboard->tradeboard.newPrice1_show->text().toDouble();
    }
    else if(2 == tradeboard->tradeboard.pricetype_comboBox->currentIndex())   //按对手价成交
    {
        if(0==tradeboard->tradeboard.direction_comboBox->currentIndex())      //买，按卖一价买
        {
            order_field.price = tradeboard->tradeboard.AskPrice1_show->text().toDouble();
        }
        else if(1==tradeboard->tradeboard.direction_comboBox->currentIndex()) //卖，按买一价卖
        {
            order_field.price = tradeboard->tradeboard.BidPrice1_show->text().toDouble();
        }
    }

    // direction
    if (0==tradeboard->tradeboard.direction_comboBox->currentIndex())
    {
        order_field.direction = THOST_FTDC_D_Buy;
    }
    else if (1==tradeboard->tradeboard.direction_comboBox->currentIndex())
    {
        order_field.direction = THOST_FTDC_D_Sell;
    }
    else
    {
        order_field.direction = 'a';
    }

    // offset
    if (0==tradeboard->tradeboard.openClose_comboBox->currentIndex())
    {
        order_field.offset = THOST_FTDC_OF_Open;
    }
    else if (1==tradeboard->tradeboard.openClose_comboBox->currentIndex())
    {
        order_field.offset = THOST_FTDC_OF_Close;
    }
    else if (2==tradeboard->tradeboard.openClose_comboBox->currentIndex())
    {
        order_field.offset = THOST_FTDC_OF_CloseToday;
    }
    else if (3==tradeboard->tradeboard.openClose_comboBox->currentIndex())
    {
        order_field.offset = THOST_FTDC_OF_CloseYesterday;
    }
    else
    {
        order_field.offset = 'a';
    }

    // 判断合约为空
    if ('a' == order_field.direction || 'a' == order_field.offset
            || abs(order_field.price - 0.0) < 0.000001 || 0 == order_field.volume)
    {
        qDebug()<<QString::fromLocal8Bit("下单数据异常");
        qDebug()<<order_field.price;
        get_ret = false;
    }
    return get_ret;

}


