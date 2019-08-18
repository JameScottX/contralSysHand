#ifndef WIFIUI_H
#define WIFIUI_H

#include <QWidget>
#include "mwificore.h"

namespace Ui {
class wifiui;
}

class wifiui : public QWidget
{

    Q_OBJECT

public:

    explicit wifiui(QWidget *parent = 0);

    ~wifiui();

private:

     Ui::wifiui *ui;
     inline  void init_();
     QString ip;
     short port;


private slots:

     void wifiConnect();
     void wifiClose();
     void wifiTcp_breakoff();
     void editedToSure();
     void msgSend();
     void msgRead();
     void wifiQuit();

};




#endif // WIFIUI_H
