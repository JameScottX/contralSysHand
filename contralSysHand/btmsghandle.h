#ifndef BTMSGHANDLE_H
#define BTMSGHANDLE_H

#include <QObject>
#include <QtBluetooth/qbluetoothglobal.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>


class btMsgHandle : public QObject
{
    Q_OBJECT
public:

    explicit btMsgHandle(QObject *parent = nullptr);

    //QList<QBluetoothHostInfo> localAdapters;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothLocalDevice *localDevice;
    QBluetoothSocket *socket;

     bool useBlueTooth=false;
     bool bltway =false;

     void StringToHex_blt(QString str, QByteArray & senddata);

private :
     char ConvertHexChar(char ch);

public slots:

};

extern  btMsgHandle *msghandle;

#endif // BTMSGHANDLE_H
