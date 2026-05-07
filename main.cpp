#include "mainwindow.h"
#include <QApplication>
#include"math/distributionmodel.h"
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    qDebug()<<DistributionModel::normalPDF(0,0,0);
    return a.exec();
}
