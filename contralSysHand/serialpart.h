#ifndef SERIALPART_H_
#define SERIALPART_H_

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class serialPart : public QObject
{

public:

    explicit serialPart(QObject *parent = nullptr);
    ~serialPart();

    QSerialPort *serial;

    void setSerial(QString portName,short baud,short data,short parity,short stop);


    void serialSendData_(QByteArray bufferdata);
    QByteArray serialGetData_(void);

    char ConvertHexChar(char ch);

    void StringToHex_(QString str, QByteArray & senddata);   //此函数在所有类模块中可以用

    bool useSerialFlag=false;
    bool serialway=false;

    //这里暂放在Serial类中
    QString checkHead;
    QString checkTail;

};

extern serialPart *mSerial;

#endif // SERIALPART_H



