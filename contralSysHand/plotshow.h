#ifndef PLOTSHOW_H
#define PLOTSHOW_H

#include <QWidget>
//#include "C:/Qt/Qt5.9.2/QCustomplot/qcustomplot/qcustomplot.h"

namespace Ui {
class plotShow;
}

class plotShow : public QWidget
{
    Q_OBJECT

public:
    explicit plotShow(QWidget *parent = 0);
    ~plotShow();

private slots:

    void plottoshow();

private:
    Ui::plotShow *ui;
};


/*************************************************/
#define		LP_ERR			unsigned char
#define		LP_NOERR		(unsigned char)0x01
#define     LP_POINTERR		(unsigned char)0x02
#define     LP_NODATA		(unsigned char)0x03
#define     LP_INPUTERR		(unsigned char)0x04
#define     LP_DATANOTENOPH	(unsigned char)0x05


#define			BIGLIN			0x01
#define			LITTERLIN		0x02

class msgGetForPlot{

public:

    msgGetForPlot();

    void msgGetHandle(QByteArray *getByte);

    QByteArray headbyte;
    QByteArray tailbyte;

    typedef struct
    {
        unsigned char *pData; //存放数据的数组
        unsigned short ReadPoint;
        unsigned short WritePoint;
        unsigned short BufferSize;
        unsigned short DataSize;

    }LoopListStruct;

    LP_ERR LoopList_Init(LoopListStruct *NewStruct,const unsigned NewBufferSize);
    LP_ERR LoopListAddDate(LoopListStruct *NewStruct,const unsigned char NewData);
    LP_ERR LoopList_GetOneData(LoopListStruct *NewStruct,unsigned char *pCh);
    LP_ERR LoopList_GetMultiData(LoopListStruct *NewStruct,unsigned char *pCh,const unsigned short Size,
                                                                const unsigned char DataType);
    LP_ERR LoopList_Delete(LoopListStruct *NewStruct);

    LoopListStruct plotgetda;
    short arraySize;


};

extern msgGetForPlot *msgfplot;


#endif // PLOTSHOW_H



