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
    ui->labelTitle->setText(m_dist->getName());
    setupSliderRanges();
    onParameterChanged();
  }
}
void DetailPage::setupSliderRanges() {
  if (!m_dist)
    return;

  switch (m_dist->getType()) {
  case DistType::Normal:
    ui->Slider1->setRange(-5, 5);
    ui->Slider1->setValue(0);
    ui->Slider2->setRange(1, 50);
    ui->Slider2->setValue(10);
    break;
  case DistType::Binomial:
    ui->Slider1->setRange(1, 50);
    ui->Slider1->setValue(10);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(50);
    break;
  case DistType::Poission:
    ui->Slider1->setRange(1, 200);
    ui->Slider1->setValue(50);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(0);
    break;
  case DistType::Uniform:
    ui->Slider1->setRange(0, 100);
    ui->Slider1->setValue(0);
    ui->Slider2->setRange(1, 100);
    ui->Slider2->setValue(100);
    break;
  case DistType::Exponential:
    ui->Slider1->setRange(1, 200);
    ui->Slider1->setValue(100);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(0);
    break;
  case DistType::StudentT:
    ui->Slider1->setRange(1, 200);
    ui->Slider1->setValue(10);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(0);
    break;
  case DistType::ChiSquare:
    ui->Slider1->setRange(1, 200);
    ui->Slider1->setValue(5);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(0);
    break;
  case DistType::Beta:
    ui->Slider1->setRange(1, 200);
    ui->Slider1->setValue(10);
    ui->Slider2->setRange(1, 200);
    ui->Slider2->setValue(10);
    break;
  case DistType::Geometric:
    ui->Slider1->setRange(1, 99);
    ui->Slider1->setValue(50);
    ui->Slider2->setRange(0, 100);
    ui->Slider2->setValue(0);
    break;
  case DistType::Hypergeometric:
    ui->Slider1->setRange(5, 100);
    ui->Slider1->setValue(40);
    ui->Slider2->setRange(1, 100);
    ui->Slider2->setValue(20);
    break;
  }
}
void DetailPage::onParameterChanged() {
  if (!m_dist)
    return;
  double val1 = ui->Slider1->value();
  double val2 = ui->Slider2->value();
  double param1 = val1;
  double param2 = val2;

  switch (m_dist->getType()) {
  case DistType::Normal:
    param1 = val1;
    param2 = val2 / 10.0;
    param2 = std::max(0.1, param2);
    break;
  case DistType::Binomial:
    param1 = std::round(val1);
    param1 = std::max(1.0, param1);
    param2 = val2 / 100.0;
    param2 = std::clamp(param2, 0.0, 1.0);
    break;
  case DistType::Poission:
    param1 = val1 / 10.0;
    param1 = std::max(0.01, param1);
    param2 = 0.0;
    break;
  case DistType::Uniform:
    param1 = val1 / 10.0;
    param2 = val2 / 10.0;
    if (param2 <= param1)
      param2 = param1 + 0.1;
    break;
  case DistType::Exponential:
    param1 = val1 / 100.0;
    param1 = std::max(0.01, param1);
    param2 = 0.0;
    break;
  case DistType::StudentT:
    param1 = val1 / 1.0; // 直接使用滑块值作为自由度
    param1 = std::max(1.0, param1);
    param2 = 0.0;
    break;
  case DistType::ChiSquare:
    param1 = val1 / 1.0; // 直接使用滑块值作为自由度
    param1 = std::max(1.0, param1);
    param2 = 0.0;
    break;
  case DistType::Beta:
    param1 = val1 / 10.0; // α 参数，范围 0.1-20
    param1 = std::max(0.01, param1);
    param2 = val2 / 10.0; // β 参数，范围 0.1-20
    param2 = std::max(0.01, param2);
    break;
  case DistType::Geometric:
    param1 = val1 / 100.0; // p 参数，范围 0.01-0.99
    param1 = std::clamp(param1, 0.01, 0.99);
    param2 = 0.0;
    break;
  case DistType::Hypergeometric:
    param1 = val1; // N 参数，范围 5-100
    param1 = std::max(5.0, param1);
    param2 = val2; // K 参数，范围 1-100
    param2 = std::min(param2, param1);
    break;
  }

  m_dist->setParameters(param1, param2);
  ui->label1->setText(
      QString("%1: %2").arg(m_dist->getParam1Name()).arg(param1));
  ui->label2->setText(
      QString("%1: %2").arg(m_dist->getParam2Name()).arg(param2));

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
    if (m_dist->getType() == DistType::Uniform ||
        m_dist->getType() == DistType::Exponential ||
        m_dist->getType() == DistType::ChiSquare ||
        m_dist->getType() == DistType::Beta) {
      ui->customPlot->xAxis->setRange(start, end);
    } else {
      // 对于正态分布、学生t分布等对称分布使用建议范围
      ui->customPlot->xAxis->setRange(start, end);
    }
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
  ui->btnToggleMode->setText(m_mode == RenderMode::PMF ? "切换至 CDF"
                                                       : "切换至 PDF");
  updatePlot();
}
DetailPage::~DetailPage() { delete ui; }
