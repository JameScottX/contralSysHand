#ifndef MWIFICORE_H
#define MWIFICORE_H

#include <QObject>

#include <QTcpSocket>
#include <QAbstractSocket>

class mwifiCore : public QTcpSocket
{
public:

    mwifiCore(QTcpSocket * parent = Q_NULLPTR);

    void StringToHex_wifi(QString str, QByteArray &senddata);
    char ConvertHexChar(char ch);

    void S_Data(QByteArray *data);
    QByteArray *Recdata;
    QByteArray R_Data();
    bool usewifi =false;
    bool wifiway=false;

};

extern mwifiCore *pWifi;


#endif // MWIFICORE_H
