#include "plotshow.h"
#include "ui_plotshow.h"

#include "serialpart.h"
#include "mwificore.h"
#include "btmsghandle.h"

msgGetForPlot *msgfplot =new msgGetForPlot();

plotShow::plotShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::plotShow)
{
    ui->setupUi(this);
}

plotShow::~plotShow()
{
    delete ui;
}


void plotShow::plottoshow(){


}



/****************************************************************************/
//定义两个校验位后面10个数据

msgGetForPlot::msgGetForPlot(){



}

void msgGetForPlot::msgGetHandle(QByteArray *getByte){

     unsigned char temp;
     //char
//    if(mSerial->serialway){

//      LoopListAddDate(plotgetda,(unsigned char *)getByte);

//    }else if(pWifi->wifiway){

//      LoopListAddDate(plotgetda,getByte);


//    }else if(msghandle->bltway){

//      LoopListAddDate(plotgetda,getByte);

//  }

}

LP_ERR msgGetForPlot::LoopList_Init(LoopListStruct *NewStruct,const unsigned NewBufferSize){

    if(NewStruct<=0){

        return LP_POINTERR;

    }

    NewStruct->BufferSize=NewBufferSize;
    NewStruct->pData=(unsigned char *)malloc(sizeof(unsigned char)* NewStruct->BufferSize);
    memset(NewStruct->pData,0,NewStruct->BufferSize);//它的作用是在一段内存块中填充一个给定的值，它是对较大的结构体或者数组进行清零操作的一种最快方法.
    NewStruct->ReadPoint=0;
    NewStruct->WritePoint=0;
    NewStruct->DataSize=0;
    return LP_NOERR;

}


LP_ERR msgGetForPlot::LoopListAddDate(LoopListStruct *NewStruct,const unsigned char NewData){

    if(NewStruct<=0){
        return LP_POINTERR;
    }

    NewStruct->pData[NewStruct->WritePoint]=NewData;

    if(NewStruct->WritePoint==NewStruct->ReadPoint){

        NewStruct->DataSize=0;
    }
    NewStruct->WritePoint++;
    NewStruct->pData++;

    if(NewStruct->WritePoint ==NewStruct->ReadPoint){

        NewStruct->WritePoint=0;
    }

    return LP_NOERR;

}

LP_ERR msgGetForPlot::LoopList_GetOneData(LoopListStruct *NewStruct,unsigned char *pCh)
{
    if (NewStruct<=0)
    {
        return LP_POINTERR;
    }
    if (NewStruct->DataSize==0)
    {
        *pCh=0;
        return LP_NODATA;
    }
    *pCh=NewStruct->pData[NewStruct->ReadPoint];
    NewStruct->ReadPoint++;
    NewStruct->DataSize--;
    if (NewStruct->ReadPoint==NewStruct->BufferSize)
    {
        NewStruct->ReadPoint=0;
    }
    return LP_NOERR;
}


LP_ERR msgGetForPlot::LoopList_GetMultiData(LoopListStruct *NewStruct,unsigned char *pCh,const unsigned short Size,
                                                            const unsigned char DataType)
{
    unsigned short tmp=Size;
    if (NewStruct<=0||pCh<=0)
    {
        return LP_POINTERR;
    }
    if (tmp>=NewStruct->DataSize)
    {
        return LP_INPUTERR;
    }
    if (tmp>NewStruct->DataSize)
    {
        return LP_DATANOTENOPH;
    }
    if(DataType==BIGLIN)
    {
        pCh=pCh+Size-1;
        while(tmp)
        {
            LoopList_GetOneData(NewStruct,pCh--);
            tmp--;
        }
    }
    else if(DataType==LITTERLIN)
    {
        while(tmp)
        {
            LoopList_GetOneData(NewStruct,pCh++);
            tmp--;
        }
    }
    return LP_NOERR;
}

LP_ERR msgGetForPlot::LoopList_Delete(LoopListStruct *NewStruct){

    if(NewStruct==NULL){

        return LP_NOERR;
    }

    free(NewStruct->pData);
    NewStruct->pData=NULL;

    return LP_NOERR;

}


