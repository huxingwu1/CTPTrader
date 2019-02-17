#ifndef DATAFROMLOCAL_H
#define DATAFROMLOCAL_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QRegExpValidator>
#include "mydatabase.h"

// 将本地数据（csv/txt）导入数据库

namespace Ui {
class DataFromLocal;
}

class DataFromLocal : public QDialog
{
    Q_OBJECT

public:
    explicit DataFromLocal(QWidget *parent = 0);
    ~DataFromLocal();

private slots:
    void on_choseFile_clicked();

    void on_toDatabase_clicked();

private:
    Ui::DataFromLocal *ui;
    MyDataBase *mydatabase;
    QMap<QString,QString> data_name;
};

#endif // DATAFROMLOCAL_H
