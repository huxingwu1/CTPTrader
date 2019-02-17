#ifndef LOGIN_OTHERACCOUNT_H
#define LOGIN_OTHERACCOUNT_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class Login_otheraccount;
}

class Login_otheraccount : public QDialog
{
    Q_OBJECT

public:
    explicit Login_otheraccount(QWidget *parent = 0);
    ~Login_otheraccount();

signals:
    void sendAccount(QString);

private slots:
    void on_pushButton_clicked();

private:
    Ui::Login_otheraccount *ui;
};

#endif // LOGIN_OTHERACCOUNT_H
