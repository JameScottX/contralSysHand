#include "contralsyshand.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ContralSysHand w;
    w.show();

    return a.exec();
}
