#include "tradeboardinfo.h"


void TradeboardInfo::DockInit(int x, int y, int w, int h)
{
    Dock->setStyleSheet("font:11pt '宋体';};");
    Dock->setFeatures(QDockWidget::AllDockWidgetFeatures);
    Dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetClosable);
    //Dock->setFont(QFont("Timers" , 9 ,  QFont::Bold));
    //Dock->setStyleSheet("font:9pt '楷体';");   //设置字体大小
    //停靠区域放在外面设置
    //Dock->setAllowedAreas(Qt::LeftDockWidgetArea |Qt::TopDockWidgetArea |Qt::RightDockWidgetArea);//Qt::TopDockWidgetArea |Qt::LeftDockWidgetArea |Qt::RightDockWidgetArea

    //添加背景图片，设置背景颜色
//    QPixmap pixmap(":/icon/formskin.png");//设定图片
//    QPalette palette;//创建一个调色板对象
//    palette.setBrush(QPalette::Window,QBrush(pixmap));//用调色板的画笔把映射到pixmap上的图片画到frame.backgroundRole()这个背景上
//    palette.setColor(QPalette::Background, Qt::gray);
//    Dock->setPalette(palette);

    //Dock->setWidget(firstTable);    //将firstTable添加进入Dock
    Dock->setGeometry(x ,y ,w ,h);
    Dock->setMinimumSize(w ,h);
    Dock->setMaximumSize(w ,h);


}

void TradeboardInfo::tradeboardInit(QWidget *parent)
{
    //第一行
    tradeboard.InstrumentID =new QLabel(QString::fromLocal8Bit("合约代码"),parent);
    tradeboard.InstrumentID->setParent(Dock);
    tradeboard.InstrumentID->setGeometry(5,30,65,20);
    tradeboard.Instrument = new QLineEdit(parent);
    tradeboard.Instrument->setParent(Dock);
    tradeboard.Instrument->setGeometry(70,30,100,20);
    // 代码框设置只能输入字母与数字
    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, tradeboard.Instrument );
    tradeboard.Instrument->setValidator( validator );

    tradeboard.UpperLimitPrice = new QLabel(QString::fromLocal8Bit("涨停价:"),parent);
    tradeboard.UpperLimitPrice->setParent(Dock);
    tradeboard.UpperLimitPrice->setGeometry(190,30,50,20);
    tradeboard.UpperLimitPrice_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.UpperLimitPrice_show->setParent(Dock);
    tradeboard.UpperLimitPrice_show->setGeometry(250,30,50,20);


    //第二行
    tradeboard.vtName =new QLabel(QString::fromLocal8Bit("    名称"),parent);
    tradeboard.vtName->setParent(Dock);
    tradeboard.vtName->setGeometry(5,60,65,20);
    tradeboard.vtName_linedit = new QLineEdit(parent);
    tradeboard.vtName_linedit->setParent(Dock);
    tradeboard.vtName_linedit->setGeometry(70,60,100,20);
    tradeboard.AskPrice1_vol = new QLabel(QString::fromLocal8Bit("卖一量:"),parent);
    tradeboard.AskPrice1_vol->setParent(Dock);
    tradeboard.AskPrice1_vol->setGeometry(190,60,50,20);
    tradeboard.AskPrice1_vol_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.AskPrice1_vol_show->setParent(Dock);
    tradeboard.AskPrice1_vol_show->setGeometry(250,60,50,20);


    //第三行
    tradeboard.openClose =new QLabel(QString::fromLocal8Bit("    开平"),parent);
    tradeboard.openClose->setParent(Dock);
    tradeboard.openClose->setGeometry(5,90,65,20);
    tradeboard.openClose_comboBox = new QComboBox(parent);
    tradeboard.openClose_comboBox->addItem(QString::fromLocal8Bit("开仓"));
    tradeboard.openClose_comboBox->addItem(QString::fromLocal8Bit("平仓"));
    tradeboard.openClose_comboBox->addItem(QString::fromLocal8Bit("平今"));
    tradeboard.openClose_comboBox->addItem(QString::fromLocal8Bit("平昨"));
    tradeboard.openClose_comboBox->setParent(Dock);
    tradeboard.openClose_comboBox->setGeometry(70,90,100,20);
    tradeboard.AskPrice1 = new QLabel(QString::fromLocal8Bit("卖一价:"),parent);
    tradeboard.AskPrice1->setParent(Dock);
    tradeboard.AskPrice1->setGeometry(190,90,50,20);
    tradeboard.AskPrice1_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.AskPrice1_show->setParent(Dock);
    tradeboard.AskPrice1_show->setGeometry(250,90,50,20);


    //第四行
    tradeboard.direction =new QLabel(QString::fromLocal8Bit("    方向"),parent);
    tradeboard.direction->setParent(Dock);
    tradeboard.direction->setGeometry(5,120,65,20);
    tradeboard.direction_comboBox = new QComboBox(parent);
    tradeboard.direction_comboBox->addItem(QString::fromLocal8Bit("买多"));
    tradeboard.direction_comboBox->addItem(QString::fromLocal8Bit("卖空"));
    tradeboard.direction_comboBox->setParent(Dock);
    tradeboard.direction_comboBox->setGeometry(70,120,100,20);
    tradeboard.newPrice = new QLabel(QString::fromLocal8Bit("最新价:"),parent);
    tradeboard.newPrice->setParent(Dock);
    tradeboard.newPrice->setGeometry(190,120,50,20);
    tradeboard.newPrice1_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.newPrice1_show->setParent(Dock);
    tradeboard.newPrice1_show->setGeometry(250,120,50,20);


    //第五行
    tradeboard.price =new QLabel(QString::fromLocal8Bit("    价格"),parent);
    tradeboard.price->setParent(Dock);
    tradeboard.price->setGeometry(5,150,65,20);
    tradeboard.price_spinbox = new QDoubleSpinBox(parent);
    tradeboard.price_spinbox->setMinimum(0);
    tradeboard.price_spinbox->setMaximum(99999.99);
    tradeboard.price_spinbox->setSingleStep(1);
    tradeboard.price_spinbox->setParent(Dock);
    tradeboard.price_spinbox->setGeometry(70,150,100,20);
    tradeboard.BidPrice1 = new QLabel(QString::fromLocal8Bit("买一价:"),parent);
    tradeboard.BidPrice1->setParent(Dock);
    tradeboard.BidPrice1->setGeometry(190,150,50,20);
    tradeboard.BidPrice1_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.BidPrice1_show->setParent(Dock);
    tradeboard.BidPrice1_show->setGeometry(250,150,50,20);


    //第六行
    tradeboard.pricetype =new QLabel(QString::fromLocal8Bit("价格类型"),parent);
    tradeboard.pricetype->setParent(Dock);
    tradeboard.pricetype->setGeometry(5,180,65,20);
    tradeboard.pricetype_comboBox = new QComboBox(parent);
    tradeboard.pricetype_comboBox->addItem(QString::fromLocal8Bit("限价"));
    tradeboard.pricetype_comboBox->addItem(QString::fromLocal8Bit("市价"));
    tradeboard.pricetype_comboBox->setParent(Dock);
    tradeboard.pricetype_comboBox->setGeometry(70,180,100,20);
    tradeboard.BidPrice1_vol = new QLabel(QString::fromLocal8Bit("买一量:"),parent);
    tradeboard.BidPrice1_vol->setParent(Dock);
    tradeboard.BidPrice1_vol->setGeometry(190,180,50,20);
    tradeboard.BidPrice1_vol_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.BidPrice1_vol_show->setParent(Dock);
    tradeboard.BidPrice1_vol_show->setGeometry(250,180,50,20);


    //第七行
    tradeboard.number =new QLabel(QString::fromLocal8Bit("    数量"),parent);
    tradeboard.number->setParent(Dock);
    tradeboard.number->setGeometry(5,210,65,20);
    tradeboard.number_spinbox = new QSpinBox(parent);
    tradeboard.number_spinbox->setMinimum(1);
    tradeboard.number_spinbox->setMaximum(99);
    tradeboard.number_spinbox->setParent(Dock);
    tradeboard.number_spinbox->setGeometry(70,210,100,20);
    tradeboard.LowerLimitPrice = new QLabel(QString::fromLocal8Bit("跌停价:"),parent);
    tradeboard.LowerLimitPrice->setParent(Dock);
    tradeboard.LowerLimitPrice->setGeometry(190,210,50,20);
    tradeboard.LowerLimitPrice_show = new QLabel(QString::fromLocal8Bit("0"),parent);
    tradeboard.LowerLimitPrice_show->setParent(Dock);
    tradeboard.LowerLimitPrice_show->setGeometry(250,210,50,20);



    QFont setFont("Times", 16, QFont::Bold);   //字体，大小，风格
    tradeboard.xiadan = new QPushButton(parent);
    tradeboard.xiadan->setParent(Dock);
    tradeboard.xiadan->setFont(setFont);
    tradeboard.xiadan->setStyleSheet("background-color: rgb(255, 128, 64);");   //设置背景颜色
    tradeboard.xiadan->setGeometry(10,250,280,70);
    tradeboard.xiadan->setText(QString::fromLocal8Bit("下单"));

    tradeboard.cancel = new QPushButton(parent);
    tradeboard.cancel->setParent(Dock);
    tradeboard.cancel->setFont(setFont);
    tradeboard.cancel->setGeometry(10,325,280,70);
    tradeboard.cancel->setStyleSheet("background-color: rgb(40, 103, 250);");   //设置背景颜色//40, 103, 250
    tradeboard.cancel->setText(QString::fromLocal8Bit("一键全撤"));

}


