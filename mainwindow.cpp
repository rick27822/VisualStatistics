#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math/distributionmodel.h"
#include<QVector>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_currentDist = new NormalDistribution(0.0, 1.0);

    setupInitialStyle();


    connect(ui->sliderMu, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);
    connect(ui->sliderSigma, &QSlider::valueChanged, this, &MainWindow::onParameterChanged);
    setupNormalPlot();
    onParameterChanged();
    this->setStyleSheet(
        "QWidget { background-color: #121212; color: white; font-family: 'Arial'; }"
        "QSlider::handle:horizontal { background: #00FFF2; width: 18px; border-radius: 9px; }"
        "QSlider::groove:horizontal { background: #333; height: 4px; }"
        );
}

void MainWindow::setupNormalPlot(){
    if (!m_currentDist) {
        m_currentDist = new NormalDistribution(0.0, 1.0);
    }
    updatePlot();
}
void MainWindow::updatePlot(){
    if (!m_currentDist) return;
    //获取范围
    QPair<double, double> range = m_currentDist->getSuggestedRange();
    double start = range.first;
    double end = range.second;

    int count = 1000;
    QVector<double> x(count), y(count);
    double step = (end - start) / (count - 1);

    for (int i = 0; i < count; ++i) {
        x[i] = start + i * step;
        y[i] = m_currentDist->calculate(x[i]);
    }

    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->rescaleAxes();
    //坐标轴范围缩放
    double maxY = ui->customPlot->yAxis->range().upper;
    if (maxY > 1.0) {
        ui->customPlot->yAxis->setRange(0, maxY * 1.2);
    } else {
        ui->customPlot->yAxis->setRange(0, 1.0);
    }
    double mu = ui->sliderMu->value() / 10.0;
    double threshold = m_viewWidth * 0.3;
    if (std::abs(mu - m_currentXViewCenter) > threshold) {
        // 更新视野中心，让视野重新对准 mu
        m_currentXViewCenter = mu;
    }
    ui->customPlot->xAxis->setRange(m_currentXViewCenter - m_viewWidth/2.0,m_currentXViewCenter + m_viewWidth/2.0);
    ui->customPlot->replot();
}
void MainWindow::onParameterChanged(){
    if (!m_currentDist) return;
    //获取滑块值
    double val1 = ui->sliderMu->value() / 10.0;
    double val2 = ui->sliderSigma->value() / 10.0;
    if(m_currentDist) {
        m_currentDist->setParameters(val1, val2);
    }
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
    ui->customPlot->addGraph();
    QPen graphPen;
    graphPen.setColor(QColor(0,255,242));
    graphPen.setWidth(3.0);
    ui->customPlot->graph(0)->setPen(graphPen);
    ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 255, 242, 30)));
    ui->customPlot->axisRect()->setupFullAxesBox(false);//去掉多余边框

}
MainWindow::~MainWindow()
{
    delete ui;
}
