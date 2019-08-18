#include "contralmodel.h"
#include "ui_contralmodel.h"
#include <QMessageBox>

contralModel::contralModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::contralModel)
{

    ui->setupUi(this);
    Init_();
    paramchg();
    mSerial->StringToHex_(param_bit_check,param_head);

}

contralModel::~contralModel()
{
    delete ui;

}

void contralModel::toMajorMenu(){

this->close();

}


void contralModel::Init_(void){

    this->setAttribute(Qt::WA_AcceptTouchEvents,true);

    ui->label->installEventFilter(this);
    ui->label_2->installEventFilter(this);
    ui->label_3->installEventFilter(this);
    ui->label_4->installEventFilter(this);
    ui->label_COMMAND0->installEventFilter(this);
    ui->label_COMMAND1->installEventFilter(this);
    ui->label_COMMAND2->installEventFilter(this);
    ui->label_Send->installEventFilter(this);
    ui->label_FORWARD->installEventFilter(this);
    ui->label_FL->installEventFilter(this);
    ui->label_FR->installEventFilter(this);
    ui->label_BL->installEventFilter(this);
    ui->label_BR->installEventFilter(this);
    ui->label_BACK->installEventFilter(this);
    ui->label_LEFT->installEventFilter(this);
    ui->label_RIGHT->installEventFilter(this);

    ui->lineEdit_param1->setAutoFillBackground(true);
    ui->lineEdit_param2->setAutoFillBackground(true);
    ui->lineEdit_param3->setAutoFillBackground(true);
    ui->lineEdit_param4->setAutoFillBackground(true);
    ui->lineEdit_param5->setAutoFillBackground(true);
    ui->lineEdit_param6->setAutoFillBackground(true);


    Param_chgShow.setColor(QPalette::Base,QColor(255,0,0));
    Param_chgShow1.setColor(QPalette::Base,QColor(0,255,0));

}

void contralModel::clearInfo(void){

    ui->textBrowser->clear();

}

QByteArray byteConfig(QString byteLoc){

    QString bytecheck = mSerial->checkHead+byteLoc;
    QByteArray tempData;

    mSerial->StringToHex_(bytecheck,tempData);
     return tempData;
}

 QByteArray byteTail(QString byteTail){


    QByteArray tempData;

    mSerial->StringToHex_(byteTail,tempData);

     return tempData;
}


void contralModel::messageSend(QString tempLoc,QByteArray data){

//    if(mSerial->serialway&msghandle->bltway){
//        QMessageBox::warning(this,"Waning","please CHOOSE one way to contral!!! :( ");
//        return;
//    }else if((mSerial->useSerialFlag|msghandle->useBlueTooth)==false){
//        QMessageBox::warning(this,"Waning","please OPEN one way to contral!!! :( ");
//        return;
//    }

    if(!mSerial->serialway&&!msghandle->bltway&&!pWifi->wifiway){

        QMessageBox::warning(this,"Waning","please OPEN one way to contral!!! :( ");
        return;
    }

    allByte=byteConfig(tempLoc)+data+byteTail(mSerial->checkTail);

    if(mSerial->serialway){

        mSerial->serialSendData_(allByte);        //串口发送

        ui->textBrowser->append(allByte.toHex());

    }else if(msghandle->bltway){


        msghandle->socket->write(allByte);       //蓝牙发送
        ui->textBrowser->append(allByte.toHex());

    }else if(pWifi->wifiway){

        pWifi->S_Data(&allByte);
        ui->textBrowser->append(allByte.toHex());

    }

}

void contralModel::paramchg(){

        if(ui->lineEdit_param1->text()=="Param1" ){

            paramReady_[0]=false;
            ui->lineEdit_param1->setPalette(Param_chgShow);

        }else if(ui->lineEdit_param1->text()==""){

            paramReady_[0]=false;
            ui->lineEdit_param1->setPalette(Param_chgShow);
        }else{

            paramReady_[0]=true;
            ui->lineEdit_param1->setPalette(Param_chgShow1);

        }

        if(ui->lineEdit_param2->text()=="Param2" ){

            paramReady_[1]=false;
            ui->lineEdit_param2->setPalette(Param_chgShow);
        }else if(ui->lineEdit_param2->text()== ""){

            paramReady_[1]=false;
               ui->lineEdit_param2->setPalette(Param_chgShow);
        }else{

             paramReady_[1]=true;
               ui->lineEdit_param2->setPalette(Param_chgShow1);
        }


        if(ui->lineEdit_param3->text()=="Param3" ){

           paramReady_[2]=false;
               ui->lineEdit_param3->setPalette(Param_chgShow);

        }else if(ui->lineEdit_param3->text()== ""){

           paramReady_[2] =false;
               ui->lineEdit_param3->setPalette(Param_chgShow);
        }else{

            paramReady_[2]=true;
               ui->lineEdit_param3->setPalette(Param_chgShow1);
        }


        if(ui->lineEdit_param4->text()=="Param4" ){

            paramReady_[3] =false;
            ui->lineEdit_param4->setPalette(Param_chgShow);

        }else if(ui->lineEdit_param4->text()==""){

            paramReady_[3]  =false;
            ui->lineEdit_param4->setPalette(Param_chgShow);
        }else{

            paramReady_[3] =true;
            ui->lineEdit_param4->setPalette(Param_chgShow1);
        }


        if(ui->lineEdit_param5->text()=="Param5" ){

            paramReady_[4]  =false;
            ui->lineEdit_param5->setPalette(Param_chgShow);

        }else if(ui->lineEdit_param5->text()==""){

            paramReady_[4]  =false;
            ui->lineEdit_param5->setPalette(Param_chgShow);
        }else{

            paramReady_[4] =true;
            ui->lineEdit_param5->setPalette(Param_chgShow1);
        }


        if(ui->lineEdit_param6->text()=="Param6" ){

            paramReady_[5] =false;
            ui->lineEdit_param6->setPalette(Param_chgShow);

        }else if(ui->lineEdit_param6->text()== ""){

            paramReady_[5] =false;
            ui->lineEdit_param6->setPalette(Param_chgShow);
        }else{

            paramReady_[5]=true;
            ui->lineEdit_param6->setPalette(Param_chgShow1);
        }
}

void f_To_C(const float& val,char hex[4]){

    const char *p =(char*)(&val);

    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
    hex[2]=*((char * )(p+2));
    hex[3]=*((char * )(p+3));

}

void L_To_C(const int& val,char hex[4]){

    const char *p =(char*)(&val);

    hex[0]=*((char * )p);
    hex[1]=*((char * )(p+1));
    hex[2]=*((char * )(p+2));
    hex[3]=*((char * )(p+3));

}


QByteArray contralModel::param1_6SendByte(){

   QByteArray temp;
   double tempget;

    for(short i=0;i<6;i++){

        if( paramReady_[i]){

         switch(i){

            case 0:

             tempget=ui->doubleSpinBox->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);

            break;

            case 1:

             tempget=ui->doubleSpinBox_2->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);
                break;


            case 2:

             tempget=ui->doubleSpinBox_3->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);

                break;

            case 3:

             tempget=ui->doubleSpinBox_4->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);

                break;

            case 4:

             tempget=ui->doubleSpinBox_5->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);

                break;

            case 5:

             tempget=ui->doubleSpinBox_6->value();

             if((int)(tempget*10000)%10==0){

                 L_To_C(tempget,byteTemp);

             }else{
                  f_To_C((float)tempget,byteTemp);
             }

             temp+=param_head+QByteArray(byteTemp);

                break;
            }

        }
    }

 return temp;
}



void contralModel::paramrng(){

    QString str = ui->lineEdit_rnum1->text();

    QString num_fup;
    QString num_fdown;
    int index = str.indexOf(' ');
    if(index==-1){
        return ;
    }
    num_fdown = str.left(index);
    num_fup = str.mid(index + 1);
    param_num1down = num_fdown.toDouble();
    param_num1up = num_fup.toDouble();

    ui->doubleSpinBox->setRange(param_num1down,param_num1up);
    ui->horizontalSlider->setRange((int)param_num1down,(int)param_num1up);

}

void contralModel::paramrng2(){

        QString str = ui->lineEdit_rnum2->text();

        QString num_fup;
        QString num_fdown;
        int index = str.indexOf(' ');
        if(index==-1){
            return ;
        }
        num_fdown = str.left(index);
        num_fup = str.mid(index + 1);
        param_num2down = num_fdown.toDouble();
        param_num2up = num_fup.toDouble();

        ui->doubleSpinBox_2->setRange(param_num2down,param_num2up);
        ui->horizontalSlider_2->setRange((int)param_num2down,(int)param_num2up);

}

void contralModel::paramrng3(){

        QString str = ui->lineEdit_rnum3->text();

        QString num_fup;
        QString num_fdown;
        int index = str.indexOf(' ');
        if(index==-1){
            return ;
        }
        num_fdown = str.left(index);
        num_fup = str.mid(index + 1);
        param_num3down = num_fdown.toDouble();
        param_num3up = num_fup.toDouble();

        ui->doubleSpinBox_3->setRange(param_num3down,param_num3up);
        ui->horizontalSlider_3->setRange((int)param_num3down,(int)param_num3up);

}
void contralModel::paramrng4(){

        QString str = ui->lineEdit_rnum4->text();

        QString num_fup;
        QString num_fdown;
        int index = str.indexOf(' ');
        if(index==-1){
            return ;
        }
        num_fdown = str.left(index);
        num_fup = str.mid(index + 1);
        param_num4down = num_fdown.toDouble();
        param_num4up = num_fup.toDouble();

        ui->doubleSpinBox_4->setRange(param_num4down,param_num4up);
        ui->verticalSlider->setRange((int)param_num4down,(int)param_num4up);

}
void contralModel::paramrng5(){

        QString str = ui->lineEdit_rnum5->text();

        QString num_fup;
        QString num_fdown;
        int index = str.indexOf(' ');
        if(index==-1){
            return ;
        }
        num_fdown = str.left(index);
        num_fup = str.mid(index + 1);
        param_num5down = num_fdown.toDouble();
        param_num5up = num_fup.toDouble();

        ui->doubleSpinBox_5->setRange(param_num5down,param_num5up);
        ui->verticalSlider_2->setRange((int)param_num5down,(int)param_num5up);

}

void contralModel::paramrng6(){

        QString str = ui->lineEdit_rnum6->text();

        QString num_fup;
        QString num_fdown;
        int index = str.indexOf(' ');
        if(index==-1){
            return ;
        }
        num_fdown = str.left(index);
        num_fup = str.mid(index + 1);
        param_num6down = num_fdown.toDouble();
        param_num6up = num_fup.toDouble();

        ui->doubleSpinBox_6->setRange(param_num6down,param_num6up);
        ui->verticalSlider_3->setRange((int)param_num6down,(int)param_num6up);

}


void contralModel::slidetoshow(){

    param_num1 = ui->doubleSpinBox->value();
    param_num2 = ui->doubleSpinBox_2->value();
    param_num3 = ui->doubleSpinBox_3->value();
    param_num4 = ui->doubleSpinBox_4->value();
    param_num5 = ui->doubleSpinBox_5->value();
    param_num6 = ui->doubleSpinBox_6->value();

    ui->horizontalSlider->setValue((int)param_num1);
    ui->horizontalSlider_2->setValue((int)param_num2);
    ui->horizontalSlider_3->setValue((int)param_num3);
    ui->verticalSlider->setValue((int)param_num4);
    ui->verticalSlider_2->setValue((int)param_num5);
    ui->verticalSlider_3->setValue((int)param_num6);

  }

void contralModel::spintoshow(){

    ui->doubleSpinBox->setValue(ui->horizontalSlider->value());
    ui->doubleSpinBox_2->setValue(ui->horizontalSlider_2->value());
    ui->doubleSpinBox_3->setValue(ui->horizontalSlider_3->value());
    ui->doubleSpinBox_4->setValue(ui->verticalSlider->value());
    ui->doubleSpinBox_5->setValue(ui->verticalSlider_2->value());
    ui->doubleSpinBox_6->setValue( ui->verticalSlider_3->value());

}



bool contralModel::eventFilter(QObject *target, QEvent *event){

    if(target==ui->label){

        if(event->type()==QEvent::MouseButtonRelease){
           messageSend("01","");
            return true;
        }else{

            return false;
        }
    }else if(target==ui->label_2){

        if(event->type()==QEvent::MouseButtonRelease){

            messageSend("02","");

            return true;

        }else{

            return false;
        }

    }else if(target==ui->label_3){

        if(event->type()==QEvent::MouseButtonRelease){

             messageSend("03","");

            return true;

        }else{

            return false;
        }


    }else if(target==ui->label_4){

        if(event->type()==QEvent::MouseButtonRelease){

           messageSend("04","");

            return true;

        }else{

            return false;
        }
    }else if(target==ui->label_COMMAND0){

        if(event->type()==QEvent::MouseButtonRelease){

           messageSend("11","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_COMMAND1){

        if(event->type()==QEvent::MouseButtonRelease){

           messageSend("12","");

            return true;

        }else{

            return false;
        }

      }else if(target==ui->label_COMMAND2){

        if(event->type()==QEvent::MouseButtonRelease){

           messageSend("13","");

            return true;

        }else{

            return false;
        }

      }else if(target==ui->label_FORWARD){

                if(event->type()==QEvent::MouseButtonPress){

                   messageSend("21","");

                    return true;

                }else{

                    return false;
                }
       }else if(target==ui->label_BACK){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("22","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_LEFT){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("23","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_RIGHT){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("24","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_FL){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("25","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_FR){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("26","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_BL){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("27","");

            return true;

        }else{

            return false;
        }
      }else if(target==ui->label_BR){

        if(event->type()==QEvent::MouseButtonPress){

           messageSend("28","");

            return true;

        }else{

            return false;
        }

      }else if(target==ui->label_Send){

        if(event->type()==QEvent::MouseButtonRelease){



           messageSend("88",param1_6SendByte());

            return true;

        }else{

            return false;
        }
      }




    return QWidget::eventFilter(target, event);
}


