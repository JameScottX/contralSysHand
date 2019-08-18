#include "wifiui.h"
#include "ui_wifiui.h"
#include "plotshow.h"


wifiui::wifiui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::wifiui)
{
    ui->setupUi(this);
    init_();
}

wifiui::~wifiui()
{

    delete ui;

}

inline void wifiui::init_(){

    ui->pushButton_open->setEnabled(false);
    ui->pushButton_close->setEnabled(false);

    ui->comboBox->setCurrentIndex(1);
}



void wifiui::wifiConnect(){

     ip = ui->lineEdit_ip->text();
     port =ui->lineEdit_port->text().toInt();

    pWifi->connectToHost(ip,port);

    connect(pWifi,SIGNAL(readyRead()),this,SLOT(msgRead()));
    connect(pWifi,SIGNAL(disconnected()),this,SLOT(wifiTcp_breakoff()));

    if(!pWifi->waitForConnected(3000)){
        ui->textEdit_Rev->setText("You do not connect to your device! :( ");
        return;
    }

    ui->pushButton_close->setEnabled(true);

    pWifi->usewifi = true;

}

void wifiui::wifiTcp_breakoff(){

    pWifi->close();
    pWifi->connectToHost(ip,port);

    if(!pWifi->waitForConnected(3000)){

        ui->textEdit_Rev->setText("You had been cut off from your device! :( ");
        ui->pushButton_open->setEnabled(true);
        ui->pushButton_close->setEnabled(false);
    }
}

void wifiui::wifiClose(){

    disconnect(pWifi,SIGNAL(readyRead()),this,SLOT(msgRead()));
    disconnect(pWifi,SIGNAL(disconnected()),this,SLOT(wifiTcp_breakoff()));

    pWifi->close();
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_close->setEnabled(false);

}


void wifiui::editedToSure(){

     if(ui->lineEdit_port->text()!=""&& ui->lineEdit_ip->text()!=""){

         ui->pushButton_open->setEnabled(true);
         ui->pushButton_close->setEnabled(false);
     }

}

void wifiui::msgSend(){

   QString S_data= ui->textEdit_Send->toPlainText();

   QByteArray tempSend;

  if(ui->comboBox->currentIndex()==0){

   pWifi->StringToHex_wifi(S_data,tempSend);
  }else{
   tempSend=S_data.toUtf8();
  }

  pWifi->S_Data(&tempSend);

}

void wifiui::msgRead(){

   QString lastStr = ui->textEdit_Rev->toPlainText();
   QByteArray tempRead;
   QByteArray bufferdata;

   bufferdata = pWifi->R_Data();

   msgfplot->msgGetHandle(&bufferdata);          //这里进行数据传输图形化显示

   if(ui->comboBox->currentIndex()==0){

       tempRead =bufferdata .toHex();

   }else{

       tempRead = bufferdata;
   }
   lastStr+=tempRead +" ";
   ui->textEdit_Rev->setText(lastStr);

}

void wifiui::wifiQuit(){

    this->close();
}

