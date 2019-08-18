#include "mwificore.h"

mwifiCore *pWifi= new mwifiCore();

mwifiCore::mwifiCore(QTcpSocket *parent):QTcpSocket(parent)
{
}

void mwifiCore::S_Data(QByteArray *data){

    this->write(*data);
}


QByteArray mwifiCore::R_Data(){

   *Recdata = this->readAll();

    return *Recdata;
}

void mwifiCore::StringToHex_wifi(QString str, QByteArray & senddata){

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


char mwifiCore::ConvertHexChar(char ch)
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



