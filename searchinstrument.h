#ifndef SEARCHINSTRUMENT_H
#define SEARCHINSTRUMENT_H

#include <QWidget>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include "publicstruct.h"

namespace Ui {
class SearchInstrument;
}

class SearchInstrument : public QWidget
{
    Q_OBJECT

public:
    explicit SearchInstrument(QWidget *parent = 0);
    ~SearchInstrument();

private slots:
    //点击按钮查询
    void on_pushButton_clicked();

    void on_lineEdit_returnPressed();
private:
    void search();
private:
    Ui::SearchInstrument *ui;
    QStringList headerName;
};

#endif // SEARCHINSTRUMENT_H
