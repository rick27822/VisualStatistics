#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math/distributionmodel.h"
#include<QVector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupNormalPlot();
}

void MainWindow::setupNormalPlot(){
    double mu = 0.0;
    double sigma =1.0;
    //采样数据
    int count =1000;
    QVector<double> x(count),y(count);
    double start = mu-5*sigma;
    double end = mu+5*sigma;
    double step = (end-start)/(count-1);

    for(int i=0;i<count;i++)
    {
        x[i]=start+i*step;
        y[i]=DistributionModel::normalPDF(x[i],mu,sigma);
    }
    //配置QCutomPlot
    ui->customPlot->addGraph();//添加曲线
    ui->customPlot->graph(0)->setData(x,y);//填充数据
    //美化(画笔与坐标轴）
    QPen pen;
    pen.setColor(QColor(40,110,255));
    pen.setWidth(2.5);
    ui->customPlot->graph(0)->setPen(pen);

    ui->customPlot->xAxis->setLabel("x(Random Variable");
    ui->customPlot->yAxis->setLabel("f(X)(Probability Density)");
    ui->customPlot->rescaleAxes();
    ui->customPlot->replot();
}
MainWindow::~MainWindow()
{
    delete ui;
}
