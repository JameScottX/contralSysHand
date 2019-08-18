#ifndef CONTRALMODEL_H
#define CONTRALMODEL_H

#include <QWidget>
#include "btmsghandle.h"
#include "serialpart.h"
#include "mwificore.h"


namespace Ui {

class contralModel;

}

class contralModel : public QWidget
{
    Q_OBJECT

public:
    explicit contralModel(QWidget *parent = 0);
    ~contralModel();

private slots:

    void toMajorMenu();
    void clearInfo(void);
    void paramchg();
    QByteArray param1_6SendByte();
    void paramrng();
    void paramrng2();
    void paramrng3();
    void paramrng4();
    void paramrng5();
    void paramrng6();

    void slidetoshow();
    void spintoshow();

private:

    Ui::contralModel *ui;
    void Init_(void);
    bool eventFilter(QObject *target, QEvent *event);
    QByteArray allByte;

    void messageSend(QString tempLoc,QByteArray data);

    bool paramReady_[6]={false};

    float param_num1=0;
    float param_num2=0;
    float param_num3=0;
    float param_num4=0;
    float param_num5=0;
    float param_num6=0;

    double param_num1up=0;
    double param_num1down=0;
    double param_num2up=0;
    double param_num2down=0;
    double param_num3up=0;
    double param_num3down=0;
    double param_num4up=0;
    double param_num4down=0;
    double param_num5up=0;
    double param_num5down=0;
    double param_num6up=0;
    double param_num6down=0;

     QPalette  Param_chgShow=palette();
     QPalette  Param_chgShow1=palette();


     char byteTemp[4];
     QString param_bit_check="EF FE";
     QByteArray param_head;
     // char rere[50]="adadaafa";
};

#endif // CONTRALMODEL_H
