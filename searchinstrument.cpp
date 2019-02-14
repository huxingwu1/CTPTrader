#include "searchinstrument.h"
#include "ui_searchinstrument.h"
#include "mainengine.h"

extern MainEngine *me;

SearchInstrument::SearchInstrument(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchInstrument)
{
    ui->setupUi(this);
    setStyleSheet("font:11pt ;background-color: rgb( 90, 90, 90);color: white;");    //字体大小颜色
    setWindowTitle(QString::fromLocal8Bit("查询合约"));
    //setGeometry(x,y,w,h);
    setMinimumSize(900,400);
    setMaximumSize(900,400);
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
    ui->tableWidget->setStyleSheet("QHeaderView::section{background-color:rgb( 69, 137, 148); color: white;};");
    ui->tableWidget->setColumnCount(headerName.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerName);
    //Table->setShowGrid(false);                                 //隐藏表格线
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//Stretch ResizeToContents);  //将表头显示在表内
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);             //选中一行
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);              //表格不可编辑
    ui->tableWidget->verticalHeader()->setHidden(true);                               // 隐藏行号
}

SearchInstrument::~SearchInstrument()
{
    delete ui;
}

void SearchInstrument::on_pushButton_clicked()
{
    this->search();
}

void SearchInstrument::on_lineEdit_returnPressed()
{
    this->search();
}

void SearchInstrument::search()
{
    QString instrumentiID = ui->lineEdit->text();
    QMap <QString, InstrumentInfo>& allInstruments = me->me_getInstrumentInfo();
    if(0==allInstruments.size())
    {
        int ret = QMessageBox::warning(this, QString::fromLocal8Bit("本地合约数据为空"),QString::fromLocal8Bit("请确认TD接口已成功连接."),QMessageBox::Ok | QMessageBox::Cancel);

    }
    else
    {
        if (allInstruments.find(instrumentiID) != allInstruments.end())
        {
            //存在
            InstrumentInfo& info = allInstruments[instrumentiID];
            //清空表格
            for(int i=ui->tableWidget->rowCount();i>=0;i--)
            {
                ui->tableWidget->removeRow(i);
            }
            //将持仓写入表中
            int row = 0;
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(info.id));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(info.name));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(info.exchangeId));
            ui->tableWidget->setItem(row, 3, new QTableWidgetItem(info.deadline.toString("yyyy.MM.dd")));
            ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(info.marginRate)));
            ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(info.multiplier)));
            ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(info.openCommission)));
            ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(info.closeCommission)));
            ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(info.closeTodayCommission)));
            ui->tableWidget->setItem(row, 9, new QTableWidgetItem(QString::number(info.minimumUnit)));
            ui->tableWidget->setItem(row, 10, new QTableWidgetItem(QString::number(info.tradable)));
        }
        else
        {
            // 合约信息table
            int ret = QMessageBox::warning(this, QString::fromLocal8Bit("查询失败"),QString(QObject::tr("please make sure the instrument id: %1 is correct?").arg(instrumentiID)),QMessageBox::Ok | QMessageBox::Cancel);
        }
    }
}
