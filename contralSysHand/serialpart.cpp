#include "serialpart.h"

serialPart *mSerial= new serialPart();

serialPart::serialPart(QObject *parent) : QObject(parent)
{

}

serialPart::~serialPart(){

    delete serial;
}

void serialPart::serialSendData_(QByteArray bufferdata)
{
    serial->write(bufferdata);
}


QByteArray serialPart::serialGetData_(void){


    QByteArray tempbuffer;

    tempbuffer=mSerial->serial->readAll();

    return tempbuffer;
}

void serialPart::StringToHex_(QString str, QByteArray & senddata)  //字符串转换成16进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

char serialPart::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
//        else return (-1);
        else return ch-ch;//不在0-f范围内的会发送成0
}


void serialPart::setSerial(QString portName,short baud,short data,short parity,short stop ){

    serial= new QSerialPort();

    serial->setPortName(portName);
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(baud);

    switch(data){

    case 5:serial->setDataBits(QSerialPort::Data5); break;
    case 6:serial->setDataBits(QSerialPort::Data6); break;
    case 7:serial->setDataBits(QSerialPort::Data7); break;
    case 8:serial->setDataBits(QSerialPort::Data8); break;
    default :break;

    }


    switch(parity){
    case 0: serial->setParity(QSerialPort::NoParity); break;
    default :break;
    }
    switch(stop){
    case 0:serial->setStopBits(QSerialPort::OneStop);break;
    case 1:serial->setStopBits(QSerialPort::TwoStop);break;
    default :break;
    }
    serial->setFlowControl(QSerialPort::NoFlowControl);

}
