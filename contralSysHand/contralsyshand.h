#ifndef CONTRALSYSHAND_H
#define CONTRALSYSHAND_H

#include <QMainWindow>

#include <QListWidgetItem>
#include <QEvent>
#include <QTouchEvent>
#include <QTimer>
#include <QThread>

#include "btmsghandle.h"
#include "QDebug"
#include "contralmodel.h"
#include "serialpart.h"
#include "wifiui.h"
#include "plotshow.h"

namespace Ui {

class ContralSysHand;

}

class ContralSysHand : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContralSysHand(QWidget *parent = 0);

    ~ContralSysHand();

private slots:

    void bltScan();

    void bltClose();
    void bltDevDiscover(const QBluetoothDeviceInfo &info);
    void itemActivated(QListWidgetItem *item);
    void bltConnected();
    void bltSendMsg();
    void bltReadMsg();
    void toMajorMenu();
    void mClearText();

    void closeMajor();
    void useoneway();
    void chooseOneWay();

protected:

     contralModel *pcontral_;
     void uiColorSet();

private:

    Ui::ContralSysHand *ui;
     QThread *bltThr;


     bool serialway = false;
     bool wifiway = false;

     inline void bltInit();
     bool eventFilter(QObject *target, QEvent *event);

/***************************************************************/
private:

    QTimer *serialRefresh;

    QString serial_portName;
    QString serial_systemLocation;
    QString serial_description;

    inline void serialInit();

private slots:

    void useSerial();
    void serialDetect();
    void serialClear();
    void openSerial();
    void serialReadData();
    void closeSerial();
    void serialSendData();
    void bitCheck();

/****************************************************************/

private:

   wifiui *mywifi;
   inline void wifiInit();
   QThread *wifiThr;
private slots:

    void wifiToShow();
/**********************************************************************/

public:



};



#endif // CONTRALSYSHAND_H
