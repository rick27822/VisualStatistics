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
    setupInitialStyle();

    connect(ui->sliderMu, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);
    connect(ui->sliderSigma, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);

    onParameterChanged();
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
void MainWindow::updatePlot(){
    int count =1000;
    QVector<double> x(count),y(count);
    double start = m_mu-5*m_sigma;
    double end = m_mu+5*m_sigma;
    double step = (end-start)/(count-1);
    for(int i=0;i<count;i++)
    {
        x[i]=start+i*step;
        y[i]=DistributionModel::normalPDF(x[i],m_mu,m_sigma);
    }
    ui->customPlot->graph(0)->setData(x,y);
    ui->customPlot->replot();
}
void MainWindow::onParameterChanged(){
    //获取滑块值
    m_mu=ui->sliderMu->value()/10.0;
    m_sigma=ui->sliderSigma->value()/10.0;
    //更新数值显示标签
    ui->labelMu->setText(QString("均值 (μ): %1").arg(m_mu));
    ui->labelSigma->setText(QString("标准差 (σ): %1").arg(m_sigma));

    updatePlot();
}
void MainWindow::setupInitialStyle(){
    ui->customPlot->setBackground(QBrush(QColor(30,30,30)));//背景
    QPen axisPen(Qt::white);//坐标轴与字体
    ui->customPlot->xAxis->setBasePen(axisPen);
    ui->customPlot->yAxis->setBasePen(axisPen);
    ui->customPlot->xAxis->setTickPen(axisPen);
    ui->customPlot->yAxis->setTickPen(axisPen);
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    //图线美化
    ui->customPlot->addGraph();
    QPen graphPen;
    graphPen.setColor(QColor(0,255,242));
    graphPen.setWidth(3.0);
    ui->customPlot->graph(0)->setPen(graphPen);

    ui->customPlot->axisRect()->setupFullAxesBox(false);//去掉多余边框

}
MainWindow::~MainWindow()
{
    delete ui;
}
