#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math/distributionmodel.h"
#include<QVector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentDist = new BinomialDistribution(5,5);//测试用

    setupInitialStyle();


    connect(ui->sliderMu, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);
    connect(ui->sliderSigma, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);
    setupPlot();
    onParameterChanged();
    this->setStyleSheet(
        "QWidget { background-color: #121212; color: white; font-family: 'Arial'; }"
        "QSlider::handle:horizontal { background: #00FFF2; width: 18px; border-radius: 9px; }"
        "QSlider::groove:horizontal { background: #333; height: 4px; }"
        );
}

void MainWindow::setupPlot(){
    if (!m_currentDist) {
        return;
    }
    updatePlot();
}
void MainWindow::updatePlot(){
    if (!m_currentDist) return;
    ui->customPlot->clearPlottables();
    //获取范围
    QPair<double, double> range = m_currentDist->getSuggestedRange();
    double start = range.first;
    double end = range.second;
     if (m_currentDist->getCategory() == DistributionCategory::Continuous) {
        // 连续分布：使用曲线图
        plotContinuousDistribution(start, end);
    } else {
        // 离散分布：使用柱形图
        plotDiscreteDistribution(start, end);
    }
    //坐标轴范围缩放
    ui->customPlot->yAxis->rescale();
    double upperBound = ui->customPlot->yAxis->range().upper;
    ui->customPlot->yAxis->setRange(0, upperBound * 1.2);
    if(m_currentDist->getCategory() == DistributionCategory::Continuous)
    {
        ui->customPlot->xAxis->setRange(-5,10);
    }
    else{
        ui->customPlot->xAxis->setRange(start,end);
    }
    ui->customPlot->replot();
}
void MainWindow::plotContinuousDistribution(double start, double end) {
    
    int count = 1000;
    QVector<double> x(count), y(count);
    double step = (end - start) / (count - 1);

    for (int i = 0; i < count; ++i) {
        x[i] = start + i * step;
        y[i] = m_currentDist->calculate(x[i]);
    }
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->rescaleAxes();
    QPen graphPen;
    graphPen.setColor(QColor(0,255,242));
    graphPen.setWidth(3.0);
    ui->customPlot->graph(0)->setPen(graphPen);
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 255, 242, 30)));
    ui->customPlot->axisRect()->setupFullAxesBox(false);//去掉多余边框
}
void MainWindow::plotDiscreteDistribution(double start, double end) {
    ui->customPlot->clearPlottables();
    QCPBars *bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    int size = end - start + 1;
    if (size <= 0) return;
    QVector<double> ticks(size), values(size);
    for (int i = 0; i < size; ++i) {
        int k = start + i;
        ticks[i] = k;
        values[i] = m_currentDist->calculate(double(k));
    }

    bars->setData(ticks, values);
    bars->setWidth(0.6);
    bars->setPen(QPen(QColor(0, 255, 242), 1.5));
    bars->setBrush(QColor(0, 255, 242, 80));

    ui->customPlot->rescaleAxes();
}
void MainWindow::onParameterChanged(){
    if (!m_currentDist) return;
    //获取滑块值
    double val1 = ui->sliderMu->value();
    double val2 = ui->sliderSigma->value();
    if(m_currentDist->getType() == DistType::Normal)
    {
        val1/=10.0;
        val2/=10.0;
    }
    if(m_currentDist) {

    }
    if(m_currentDist->getType() == DistType::Binomial)
    {
        val2/=100;
    }
    m_currentDist->setParameters(val1, val2);
    //更新数值显示标签
    ui->labelMu->setText(QString("%1: %2").arg(m_currentDist->getParam1Name()).arg(val1));
    ui->labelSigma->setText(QString("%1: %2").arg(m_currentDist->getParam2Name()).arg(val2));

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

    //连续分布：使用曲线图


    if (m_currentDist->getCategory() == DistributionCategory::Continuous){
        ui->customPlot->addGraph();
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
