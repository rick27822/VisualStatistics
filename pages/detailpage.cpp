#include "detailpage.h"
#include "ui_detailpage.h"

DetailPage::DetailPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::DetailPage) {
  ui->setupUi(this);

  setupInitialStyle();

  connect(ui->Slider1, &QSlider::valueChanged, this,
          &DetailPage::onParameterChanged);
  connect(ui->Slider2, &QSlider::valueChanged, this,
          &DetailPage::onParameterChanged);
  connect(ui->btnToggleMode, &QPushButton::clicked, this,
          &DetailPage::toggleRenderMode);
  connect(ui->backRequested, &QPushButton::clicked, this,
          &DetailPage::backRequested);
}
void DetailPage::setupInitialStyle() {
  // 3b1b 风格：深色背景，青色高亮
  ui->customPlot->setBackground(QBrush(QColor(18, 18, 18)));
  QCPAxis *xAxis = ui->customPlot->xAxis;
  QCPAxis *yAxis = ui->customPlot->yAxis;

  QPen axisPen(Qt::white);
  axisPen.setWidthF(1.2);
  xAxis->setBasePen(axisPen);
  yAxis->setBasePen(axisPen);
  xAxis->setTickLabelColor(Qt::white);
  yAxis->setTickLabelColor(Qt::white);

  // 隐藏不必要的边框线，增加呼吸感
  ui->customPlot->axisRect()->setupFullAxesBox(false);
}
void DetailPage::setDistribution(BaseDistribution *dist) {
  m_dist = dist;
  if (m_dist) {
    ui->labelTitle->setText(m_dist->getParam1Name().split(" ").first() +" 分布"); // 假设名称
    onParameterChanged();             // 初始化绘图
  }
}
void DetailPage::onParameterChanged() {
  if (!m_dist)
    return;
  // 获取滑块值
  double val1 = ui->Slider1->value();
  double val2 = ui->Slider2->value();
  if (m_dist->getType() == DistType::Normal) {
    val1 /= 10.0;
    val2 /= 10.0;
  }
  if (m_dist->getType() == DistType::Binomial) {
    val2 /= 100;
  }
  m_dist->setParameters(val1, val2);
  // 更新数值显示标签
  ui->label1->setText(QString("%1: %2").arg(m_dist->getParam1Name()).arg(val1));
  ui->label2->setText(QString("%1: %2").arg(m_dist->getParam2Name()).arg(val2));

  updatePlot();
}
void DetailPage::updatePlot() {
  if (!m_dist)
    return;
  ui->customPlot->clearPlottables();
  // 获取范围
  QPair<double, double> range = m_dist->getSuggestedRange();
  double start = range.first;
  double end = range.second;
  if (m_dist->getCategory() == DistributionCategory::Continuous) {
    // 连续分布：使用曲线图
    plotContinuousDistribution(start, end);
  } else {
    // 离散分布：使用柱形图
    plotDiscreteDistribution(start, end);
  }
  // 坐标轴范围缩放
  ui->customPlot->yAxis->rescale();
  double upperBound = ui->customPlot->yAxis->range().upper;
  ui->customPlot->yAxis->setRange(0, upperBound * 1.2);
  if (m_dist->getCategory() == DistributionCategory::Continuous) {
    ui->customPlot->xAxis->setRange(-5, 10);
  } else {
    ui->customPlot->xAxis->setRange(start, end);
  }
  ui->customPlot->replot();
}
void DetailPage::plotContinuousDistribution(double start, double end) {

  int count = 1000;
  QVector<double> x(count), y(count);
  double step = (end - start) / (count - 1);
  if (m_mode == RenderMode::PMF) {
    for (int i = 0; i < count; ++i) {
      x[i] = start + i * step;
      y[i] = m_dist->calculate(x[i]);
    }
  } else {
    for (int i = 0; i < count; ++i) {
      x[i] = start + i * step;
      y[i] = m_dist->calculateCDF(x[i]);
    }
  }

  ui->customPlot->addGraph();
  ui->customPlot->graph(0)->setData(x, y);
  ui->customPlot->graph(0)->rescaleAxes();
  QPen graphPen;
  graphPen.setColor(QColor(0, 255, 242));
  graphPen.setWidth(3.0);
  ui->customPlot->graph(0)->setPen(graphPen);
  ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 255, 242, 30)));
  ui->customPlot->axisRect()->setupFullAxesBox(false); // 去掉多余边框
}
void DetailPage::plotDiscreteDistribution(double start, double end) {
  Q_UNUSED(start);
  ui->customPlot->clearPlottables();
  QCPBars *bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);

  int n = static_cast<int>(end - 1);
  QVector<double> ticks, values;
  double cumulativeSum = 0.0;

  for (int k = 0; k <= n; ++k) {
    ticks.append(k);
    double p_k = m_dist->calculate(static_cast<double>(k));

    if (m_mode == RenderMode::PMF) {
      values.append(p_k);
    } else {
      cumulativeSum += p_k;
      values.append(cumulativeSum);
    }
  }

  bars->setData(ticks, values);

  bars->setPen(QPen(QColor(0, 255, 242), 1.2));
  if (m_mode == RenderMode::PMF) {
    bars->setBrush(QColor(0, 255, 242, 80));
  } else {
    bars->setBrush(QColor(0, 255, 242, 160));
  }

  ui->customPlot->rescaleAxes();

  if (m_mode == RenderMode::CDF) {
    ui->customPlot->yAxis->setRange(0, 1.1);
  }
  ui->customPlot->replot();
}
void DetailPage::toggleRenderMode() {
  m_mode = (m_mode == RenderMode::PMF) ? RenderMode::CDF : RenderMode::PMF;
  ui->btnToggleMode->setText(m_mode == RenderMode::PMF ? "切换至 CDF" : "切换至 PDF");
  updatePlot();
}
DetailPage::~DetailPage() { delete ui; }
