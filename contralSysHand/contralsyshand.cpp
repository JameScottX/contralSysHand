#include "contralsyshand.h"
#include "ui_contralsyshand.h"


static const QLatin1String serviceUuid("00001101-0000-1000-8000-00805F9B34FB");

ContralSysHand::ContralSysHand(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ContralSysHand)
{
    ui->setupUi(this);


    bltInit();         //蓝牙初始化
    serialInit();      //串口初始化
    uiColorSet();      //界面初始化
    bitCheck();
    wifiInit();
    useoneway();

}


ContralSysHand::~ContralSysHand()
{
    delete ui;
}

inline void ContralSysHand::bltInit(){


   // msghandle =new btMsgHandle();      //这里不依据父级，也就是ContralSysHand

    connect(ui->pbConnect,SIGNAL(released()),this,SLOT(bltScan()));

    if( msghandle->localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff )  {

            ui->closeBlt->setEnabled(false);

    }else {

            ui->closeBlt->setEnabled(true);
    }

    ui->bltSend->setEnabled(false);
    /**********以下为事件过滤器****************************/

    ui->label_tab1->installEventFilter(this);
    ui->label_tab2->installEventFilter(this);
    ui->label_tab3->installEventFilter(this);
    ui->label_tab4->installEventFilter(this);
    ui->label_pcontral1->installEventFilter(this);

    /****************************************************/
    pcontral_ =new contralModel();

    ui->radioButton_blt->setChecked(false);
    ui->radioButton_ser->setChecked(false);
    ui->radioButton_wifi->setChecked(false);
    ui->radioButton_else->hide();

}

void ContralSysHand::bltDevDiscover(const QBluetoothDeviceInfo &info){

    QString label = QString("%1 %2").arg(info.address().toString()).arg(info.name());

    QList<QListWidgetItem *> items = ui->bltListWid->findItems(label, Qt::MatchExactly);
    if (items.empty()) {
        QListWidgetItem *item = new QListWidgetItem(label);
        QBluetoothLocalDevice::Pairing pairingStatus = msghandle->localDevice->pairingStatus(info.address());
        if (pairingStatus == QBluetoothLocalDevice::Paired || pairingStatus == QBluetoothLocalDevice::AuthorizedPaired )
            item->setTextColor(QColor(Qt::green));
        else
            item->setTextColor(QColor(Qt::black));

        ui->bltListWid->addItem(item);
    }
}

void ContralSysHand::bltClose(){

    msghandle->socket->disconnectFromService();

    msghandle->localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);

    ui->closeBlt->setEnabled(false);
    ui->bltListWid->clear();

    msghandle-> discoveryAgent->stop();

    msghandle->useBlueTooth=false;

    ui->bltSend->setEnabled(false);

}

void ContralSysHand::bltScan(){

    if(msghandle->localDevice->hostMode() == QBluetoothLocalDevice::HostPoweredOff) {   //这里检测蓝牙是否打开
      msghandle->localDevice->powerOn();
      ui->closeBlt->setEnabled(true);
    }
       if(ui->checkBox->isChecked()==true){     //这里设置是否可被发现
         msghandle->localDevice->setHostMode( QBluetoothLocalDevice::HostDiscoverable);
       }else{
         msghandle->localDevice->setHostMode( QBluetoothLocalDevice::HostConnectable);
       }


      msghandle-> discoveryAgent->setInquiryType(QBluetoothDeviceDiscoveryAgent::GeneralUnlimitedInquiry);
      msghandle-> discoveryAgent->start();

       connect(msghandle->discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
               this, SLOT(bltDevDiscover(QBluetoothDeviceInfo)));

       connect(msghandle->discoveryAgent, SIGNAL(finished()), this, SLOT(scanFinished()));

       connect(ui->bltListWid, SIGNAL(itemActivated(QListWidgetItem*)),
               this, SLOT(itemActivated(QListWidgetItem*)));

       ui->recBrowser->setText("connecting......");

}

void ContralSysHand::itemActivated(QListWidgetItem *item){

    QString text =item->text();

    int index =text.indexOf(' ');
    if(index==-1){
        return;
    }

    QBluetoothAddress tempAddress(text.left(index));
    QString name(text.mid(index + 1));
    QString temp = QString("%1").arg(tempAddress.toString());

    ui->recBrowser->append("You has choice the bluetooth address is "+temp);
    ui->recBrowser->append("The device is connneting.... "+name);

    msghandle->socket->connectToService(tempAddress, QBluetoothUuid(serviceUuid) ,QIODevice::ReadWrite);

    connect(msghandle->socket,SIGNAL(connected()),this,SLOT(bltConnected()));


}


void ContralSysHand::bltConnected(){


    msghandle->moveToThread(bltThr);
    bltThr->start();

    ui->recBrowser->append("successful connected~_~");

    connect(msghandle->socket,SIGNAL(readyRead()),this,SLOT(bltReadMsg()));

    msghandle->useBlueTooth=true;       //这里使用蓝牙发送

    ui->bltSend->setEnabled(true);

}

void ContralSysHand::bltSendMsg(){

   QString tempbur=  ui->textEdit->toPlainText();
   QString tempdata2= ui->textBrowser_2->toPlainText();

   QByteArray tempSend;

   if(ui->comboBox_blt_show->currentIndex()==0){

   msghandle->StringToHex_blt(tempbur,tempSend);
   ui->textBrowser_2->setText(tempdata2+" "+tempSend.toHex());

   }else{

      tempSend=tempbur.toUtf8();
      ui->textBrowser_2->setText(tempdata2+" "+tempSend);
   }

    msghandle->socket->write(tempSend);

}

void ContralSysHand::bltReadMsg(){

    QString temp = ui->recBrowser->toPlainText();
    QByteArray bufferdata =  msghandle->socket->readAll();

    msgfplot->msgGetHandle(&bufferdata);          //这里进行数据传输图形化显示

    QByteArray tempbuffer;

     if(ui->comboBox_blt_show->currentIndex()==0){

         tempbuffer=bufferdata.toHex();

     }else{

         tempbuffer= bufferdata;
     }

      QString temp2 = temp+ " "+ QString(tempbuffer);
      ui->recBrowser->setText(temp2);


}

void ContralSysHand::toMajorMenu(){

    ui->tabWidget->setCurrentIndex(0);

}

void ContralSysHand::mClearText(){

    ui->recBrowser->clear();
    ui->textBrowser_2->clear();

}

/**********************************************************************************************************************/

inline void ContralSysHand::serialInit(){

    ui->serial_close->setEnabled(false);
    ui->serial_send_button->setEnabled(false);
    ui->serial_open->setEnabled(false);
}

void ContralSysHand::useSerial(){


    if(!ui->checkBox_2->isChecked()){

        if(mSerial->serial->isOpen()){
            closeSerial();
        }

        ui->serial_open->setEnabled(false);
        ui->serialInformation->clear();
        ui->serial_port->clear();

    }else{

        serialRefresh =new QTimer();
        connect(serialRefresh,SIGNAL(timeout()),this,SLOT(serialDetect()));
        serialRefresh->start(2000);

        }

}

void ContralSysHand::serialClear(){

    ui->browser_serial_receive->clear();
    ui->browser_serial_send->clear();

}

void ContralSysHand::serialDetect(){

    ui->serialInformation->clear();
    short temp = ui->serial_port->currentIndex();
    ui->serial_port->clear();

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){

        serial_portName=info.portName();
        serial_systemLocation=info.systemLocation();
        serial_description=info.description();

        QString tempOut = serial_portName+serial_systemLocation+serial_description;

        ui->serialInformation->append(tempOut);
        ui->serial_port->addItem(serial_portName);
        ui->serial_port->setCurrentIndex(temp);

        if(!info.isNull()&&!mSerial->useSerialFlag){

            ui->serial_open->setEnabled(true);
        }

    }
}

void ContralSysHand::openSerial(){

   mSerial->setSerial(ui->serial_port->currentText(),ui->serial_bund->currentText().toInt(),
                       ui->serial_data->currentText().toInt(),ui->serial_parity->currentIndex(),
                       ui->serial_stopbit->currentIndex());

    connect(mSerial->serial,SIGNAL(readyRead()),this,SLOT(serialReadData()));

    mSerial->useSerialFlag=true;       //这里使用串口发送

    ui->serial_send_button->setEnabled(true);
    ui->serial_close->setEnabled(true);
    ui->serial_open->setEnabled(false);
    ui->serialInformation->append(" you have successed connect the serial! ");

}

void ContralSysHand::serialReadData(){

    QByteArray tempbuffer;
    QByteArray bufferdata;
    bufferdata =mSerial->serialGetData_();

    msgfplot->msgGetHandle(&bufferdata);          //这里进行数据传输图形化显示

    if(ui->comboBox_serial_show->currentIndex()==0){

        tempbuffer=bufferdata.toHex();


    }else{

        tempbuffer=bufferdata;
    }

    if(!tempbuffer.isEmpty()){


     QString tempbur = ui->browser_serial_receive->toPlainText();

        tempbur+=tempbuffer+" ";

        ui->browser_serial_receive->setText(tempbur);
    }

}



void ContralSysHand::serialSendData(){

     QString tempbur=  ui->textEdit_serial->toPlainText();
     QString tempdata2= ui->browser_serial_send->toPlainText();

     QByteArray tempSend;

     if(ui->comboBox_serial_show->currentIndex()==0){

     mSerial->StringToHex_(tempbur,tempSend);
     ui->browser_serial_send->setText(tempdata2+" "+tempSend.toHex());

     }else{

        tempSend=tempbur.toUtf8();
        ui->browser_serial_send->setText(tempdata2+" "+tempSend);
     }

       mSerial->serialSendData_(tempSend);

}

void ContralSysHand::closeSerial(){

    delete serialRefresh;

    mSerial->serial->clear();
    mSerial->serial->close();
    mSerial->serial->deleteLater();

    ui->serial_close->setEnabled(false);
    ui->serial_open->setEnabled(true);
    ui->serial_send_button->setEnabled(false);

    mSerial->useSerialFlag=false;

}

void ContralSysHand::bitCheck(){       //这里设置校验位

    if(ui->checkBox_3->isChecked()){
        QByteArray temp;
        QString temp2;
        temp2 =ui->textEdit_3->toPlainText();
        //mSerial-> StringToHex_(ui->textEdit_3->toPlainText(),temp);
        mSerial->checkHead=temp2;
        mSerial->StringToHex_(temp2,temp);
        msgfplot->headbyte=temp;


       temp2 =ui->textEdit_4->toPlainText();
       // mSerial-> StringToHex_(ui->textEdit_4->toPlainText(),temp);
        mSerial->checkTail =temp2;
        mSerial->StringToHex_(temp2,temp);
        msgfplot->tailbyte=temp;

    }else{

         // mSerial-> StringToHex_("FE FF",temp);
          mSerial->checkHead="FE FF";
          mSerial->checkTail="";
    }
}

void ContralSysHand::closeMajor(){

    if(msghandle->useBlueTooth){
        bltClose();
    }
      msghandle->localDevice->setHostMode(QBluetoothLocalDevice::HostPoweredOff);

      if(mSerial->useSerialFlag){
          closeSerial();
      }

      this->close();
}


void ContralSysHand::useoneway(){

    if(ui->tabWidget->currentIndex()==3){

            if(mSerial->useSerialFlag){

                ui->radioButton_ser->setEnabled(true);
                ui->radioButton_ser->setChecked(true);

            }else{


                ui->radioButton_ser->setEnabled(false);

            }

            if(msghandle->useBlueTooth){

                ui->radioButton_blt->setEnabled(true);
                ui->radioButton_blt->setChecked(true);

            }else{


               ui->radioButton_blt->setEnabled(false);

            }

            if(pWifi->usewifi){

                ui->radioButton_wifi->setEnabled(true);
                ui->radioButton_wifi->setChecked(true);

            }else{


               ui->radioButton_wifi->setEnabled(false);

            }

            if(!mSerial->useSerialFlag&&!msghandle->useBlueTooth&&!pWifi->usewifi){

                ui->radioButton_else->setChecked(true);

            }
         chooseOneWay();
    }
}


void ContralSysHand::chooseOneWay(){

    if(ui->radioButton_blt->isChecked()){

        msghandle->bltway=true;

    }else{
        msghandle->bltway=false;
    }

    if(ui->radioButton_ser->isChecked()){


        mSerial->serialway =true;

    }else{

        mSerial->serialway =false;
    }

    if(ui->radioButton_wifi->isChecked()){

        pWifi->wifiway=true;

    }else{

        pWifi->wifiway=false;

    }
}

/***************************************************************************************************/

inline void ContralSysHand::wifiInit(){


    mywifi=new wifiui();
    wifiThr =new QThread();
}

void ContralSysHand::wifiToShow(){

    mywifi->moveToThread(wifiThr);
    wifiThr->start();
    mywifi->show();

}


/***************************************************************************************************/

bool ContralSysHand::eventFilter(QObject *target, QEvent *event){

    if(target==ui->label_tab1){

        if(event->type()==QEvent::MouseButtonRelease){


            ui->tabWidget->setCurrentIndex(1);

            return true;

        }else{

            return false;
        }

    }else if(target==ui->label_tab2){

        if(event->type()==QEvent::MouseButtonRelease){


            ui->tabWidget->setCurrentIndex(2);

            return true;

        }else{

            return false;
        }

    }else if(target==ui->label_tab3){

        if(event->type()==QEvent::MouseButtonRelease){


            ui->tabWidget->setCurrentIndex(3);

            return true;

        }else{

            return false;
        }

    }else if(target==ui->label_tab4){

        if(event->type()==QEvent::MouseButtonRelease){


            ui->tabWidget->setCurrentIndex(4);

            return true;

        }else{

            return false;
        }

    }


    if(target ==ui->label_pcontral1){

        if(event->type()==QEvent::MouseButtonRelease){

       pcontral_->show();

            return true;

        }else{

            return false;
        }

    }

   return QMainWindow::eventFilter(target, event);
}




void ContralSysHand::uiColorSet(){

//    this->setAutoFillBackground(true);
//    ui->label_tab1->setAutoFillBackground(true);
//    ui->label_tab2->setAutoFillBackground(true);
//    ui->label_tab3->setAutoFillBackground(true);
//    ui->label_tab4->setAutoFillBackground(true);
//    ui->pushButton_2->setAutoFillBackground(true);
//    ui->pbConnect->setAutoFillBackground(true);
//    ui->closeBlt->setAutoFillBackground(true);
//    ui->pushButton_4->setAutoFillBackground(true);
//    ui->bltSend->setAutoFillBackground(true);
//    ui->pushButton->setAutoFillBackground(true);
//    ui->pushButton_5->setAutoFillBackground(true);
//    ui->recBrowser->setAutoFillBackground(true);
//    ui->label_pcontral1->setAutoFillBackground(true);
//    ui->label_pcontral2->setAutoFillBackground(true);
//    ui->label_pcontral3->setAutoFillBackground(true);
//    ui->label_pcontral4->setAutoFillBackground(true);


//    ui->serial_port->setAutoFillBackground(true);
//    ui->serial_bund->setAutoFillBackground(true);
//    ui->serial_data->setAutoFillBackground(true);
//    ui->serial_parity->setAutoFillBackground(true);
//    ui->serial_stopbit->setAutoFillBackground(true);
//    ui->comboBox_serial_show->setAutoFillBackground(true);

//    ui->label_3->setAutoFillBackground(true);
//    ui->label_4->setAutoFillBackground(true);
//    ui->label_5->setAutoFillBackground(true);
//    ui->label_6->setAutoFillBackground(true);
//    ui->label_10->setAutoFillBackground(true);


//    QPalette  pbrush_lev1= palette();

//    pbrush_lev1.setColor(QPalette::Background,QColor(80,248,243));
//    pbrush_lev1.setColor(QPalette::Text,QColor(255,0,0));
//    pbrush_lev1.setColor(QPalette::Base,QColor(50,50,50));
//    pbrush_lev1.setColor(QPalette::Foreground,QColor(255,72,72));
//    pbrush_lev1.setColor(QPalette::Button,QColor(25,210,79));

//    ui->textBrowser_2->setPalette(pbrush_lev1);



//    this->setPalette(pbrush_lev1);
//    ui->label_tab1->setPalette(pbrush_lev1);
//    ui->recBrowser->setPalette(pbrush_lev1);
//    ui->pushButton_5->setPalette(pbrush_lev1);

//    ui->serial_port->setPalette(pbrush_lev1);
//    ui-> serial_bund->setPalette(pbrush_lev1);
//    ui->serial_data->setPalette(pbrush_lev1);
//    ui->serial_parity->setPalette(pbrush_lev1);
//    ui->serial_stopbit->setPalette(pbrush_lev1);

//    QPalette  pbrush_lev7= palette();
//    pbrush_lev7.setColor(QPalette::Background,QColor(255,156,120));
//    pbrush_lev7.setColor(QPalette::Text,QColor(255,255,0));
//    pbrush_lev7.setColor(QPalette::Button,QColor(200,0,19));
//    ui->label_pcontral3->setPalette(pbrush_lev7);

//    ui->label_3->setPalette(pbrush_lev7);
//    ui->label_4->setPalette(pbrush_lev7);
//    ui->label_5->setPalette(pbrush_lev7);
//    ui->label_6->setPalette(pbrush_lev7);
//    ui->label_10->setPalette(pbrush_lev7);

//    QPalette  pbrush_lev2= palette();
//    pbrush_lev2.setColor(QPalette::Background,QColor(136,255,17));
//    pbrush_lev2.setColor(QPalette::ButtonText,QColor(255,255,0));
//    pbrush_lev2.setColor(QPalette::Button,QColor(136,255,17));

//    ui->label_tab2->setPalette(pbrush_lev2);
//    ui->pbConnect->setPalette(pbrush_lev2);
//    ui->label_pcontral4->setPalette(pbrush_lev2);

//    QPalette  pbrush_lev3= palette();
//    pbrush_lev3.setColor(QPalette::Background,QColor(255,0,128));
//    pbrush_lev3.setColor(QPalette::Button,QColor(255,0,128));
//    pbrush_lev3.setColor(QPalette::Text,QColor(248,14,80));
//    ui->label_tab3->setPalette(pbrush_lev3);
//    ui->closeBlt->setPalette(pbrush_lev3);
//    ui->label_pcontral2->setPalette(pbrush_lev3);
//    ui->comboBox_serial_show->setPalette(pbrush_lev3);
//    QPalette  pbrush_lev4= palette();
//    pbrush_lev4.setColor(QPalette::Background,QColor(0,0,255));
//    pbrush_lev4.setColor(QPalette::Text,QColor(48,189,59));
//    pbrush_lev4.setColor(QPalette::Button,QColor(0,0,255));
//    ui->pushButton_4->setPalette(pbrush_lev4);
//    ui->label_tab4->setPalette(pbrush_lev4);
//     ui->label_pcontral1->setPalette(pbrush_lev4);
//     ui->pushButton->setPalette(pbrush_lev4);


//    QPalette  pbrush_lev5= palette();
//    pbrush_lev5.setColor(QPalette::Background,QColor(255,128,0));
//    pbrush_lev5.setColor(QPalette::Foreground,QColor(255,0,255));
//    ui->label_tab1->setPalette(pbrush_lev5);
//    ui->bltSend->setPalette(pbrush_lev5);
//    ui->pushButton_2->setPalette(pbrush_lev5);
//    QPalette  pbrush_lev6= palette();
//    pbrush_lev6.setColor(QPalette::Background,QColor(0,0,0));
//    pbrush_lev6.setColor(QPalette::Foreground,QColor(255,0,255));

//    ui->tabWidget->setPalette(pbrush_lev6);


//    QPalette  pbrush_lev8= palette();
//    pbrush_lev8.setColor(QPalette::Base,QColor(255,0,255));
//    ui->comboBox_blt_show->setPalette(pbrush_lev8);


}


