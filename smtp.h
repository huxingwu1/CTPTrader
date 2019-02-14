#ifndef SMTP_H
#define SMTP_H

#include <QtCore>
#include <QCoreApplication>
#include <QTcpSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QAbstractSocket>
#include <QDateTime>
#include <QDate>
#include <QLocale>
#include <QObject>


class Smtp : public QObject
{
    Q_OBJECT

public:
    Smtp(QString smtphost, QString smtpusername, QString smtppass);
    ~Smtp();
    bool Send( const QString &to,const QString &subject, const QString &body );
    int linesend;
    QStringList ErrorMSG;
    bool ReadLiner();
    void ErrorCloseAll();

signals:
    void status( const QString &);
    void SendLine();
public slots:
    bool PutSendLine();
private:
    bool isconnect;
    QString smtphost;
    QString smtpusername;
    QString smtppass;
    QString message;
    QString output;
    QString RemoteServerName;
    QString mailstatus;
    QTextStream *t;
    QTcpSocket *smtpsocket;
    QString from;
    QString rcpt;
    QString response;
    QString SendLineAndGrab(QString senddata);
    int Timeout;
    QString encodeBase64( QString xml );
    QString decodeBase64( QString xml );
    int dateswap(QString form, uint unixtime );
    QString TimeStampMail();
};

#endif // SMTP_H
