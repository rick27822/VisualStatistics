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
  connect(ui->Slider3, &QSlider::valueChanged, this,
          &DetailPage::onParameterChanged);
  connect(ui->btnToggleMode, &QPushButton::clicked, this,
          &DetailPage::toggleRenderMode);
  connect(ui->btnBack, &QPushButton::clicked, this,
          &DetailPage::backRequested);

  ui->Slider3->hide();
  ui->label3->hide();
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
    updateDescription();
  }
}
void DetailPage::updateDescription() {
  if (!m_dist)
    return;
  QString content = QString("<h3>%1</h3>")
                        .arg(m_dist->getName()) +
                    QString("<p><strong>函数表达式：</strong>%1</p>")
                        .arg(m_dist->getFunctionExpression()) +
                    QString("<p><strong>参数意义：</strong>%1</p>")
                        .arg(m_dist->getDescription()) +
                    QString("<p><strong>使用场景：</strong>%1</p>")
                        .arg(m_dist->getUsageScenario());
  ui->textBrowser->setHtml(content);
}
void DetailPage::setupSliderRanges() {
  if (!m_dist)
    return;

  int paramCount = m_dist->getParamCount();
  QList<QString> paramNames = m_dist->getParamNames();
  QList<double> paramDefaults = m_dist->getParamDefaults();
  QList<QPair<double, double>> paramRanges = m_dist->getParamRanges();

  ui->Slider1->show();
  ui->label1->show();
  ui->Slider2->hide();
  ui->label2->hide();
  ui->Slider3->hide();
  ui->label3->hide();

  if (paramCount >= 1) {
    ui->label1->setText(paramNames[0]);
    double min1 = paramRanges[0].first;
    double max1 = paramRanges[0].second;
    double default1 = paramDefaults[0];

    int sliderMax = 100;
    if (max1 - min1 <= 10)
      sliderMax = 100;
    else if (max1 - min1 <= 100)
      sliderMax = 100;
    else
      sliderMax = 200;

    ui->Slider1->setRange(0, sliderMax);
    ui->Slider1->setValue(
        static_cast<int>((default1 - min1) / (max1 - min1) * sliderMax));
  }

  if (paramCount >= 2) {
    ui->Slider2->show();
    ui->label2->show();
    ui->label2->setText(paramNames[1]);
    double min2 = paramRanges[1].first;
    double max2 = paramRanges[1].second;
    double default2 = paramDefaults[1];

    int sliderMax = 100;
    if (max2 - min2 <= 10)
      sliderMax = 100;
    else if (max2 - min2 <= 100)
      sliderMax = 100;
    else
      sliderMax = 200;

    ui->Slider2->setRange(0, sliderMax);
    ui->Slider2->setValue(
        static_cast<int>((default2 - min2) / (max2 - min2) * sliderMax));
  }

  if (paramCount >= 3) {
    ui->Slider3->show();
    ui->label3->show();
    ui->label3->setText(paramNames[2]);
    double min3 = paramRanges[2].first;
    double max3 = paramRanges[2].second;
    double default3 = paramDefaults[2];

    int sliderMax = 100;
    if (max3 - min3 <= 10)
      sliderMax = 100;
    else if (max3 - min3 <= 100)
      sliderMax = 100;
    else
      sliderMax = 200;

    ui->Slider3->setRange(0, sliderMax);
    ui->Slider3->setValue(
        static_cast<int>((default3 - min3) / (max3 - min3) * sliderMax));
  }
}
void DetailPage::onParameterChanged() {
  if (!m_dist)
    return;

  int paramCount = m_dist->getParamCount();
  QList<QString> paramNames = m_dist->getParamNames();
  QList<QPair<double, double>> paramRanges = m_dist->getParamRanges();

  QList<double> params;

  if (paramCount >= 1) {
    double min1 = paramRanges[0].first;
    double max1 = paramRanges[0].second;
    double val1 = ui->Slider1->value();
    double param1 = min1 + (val1 / ui->Slider1->maximum()) * (max1 - min1);
    params.append(param1);
    ui->label1->setText(
        QString("%1: %2").arg(paramNames[0]).arg(param1, 0, 'f', 2));
  }

  if (paramCount >= 2) {
    double min2 = paramRanges[1].first;
    double max2 = paramRanges[1].second;
    double val2 = ui->Slider2->value();
    double param2 = min2 + (val2 / ui->Slider2->maximum()) * (max2 - min2);
    params.append(param2);
    ui->label2->setText(
        QString("%1: %2").arg(paramNames[1]).arg(param2, 0, 'f', 2));
  }

  if (paramCount >= 3) {
    double min3 = paramRanges[2].first;
    double max3 = paramRanges[2].second;
    double val3 = ui->Slider3->value();
    double param3 = min3 + (val3 / ui->Slider3->maximum()) * (max3 - min3);
    params.append(param3);
    ui->label3->setText(
        QString("%1: %2").arg(paramNames[2]).arg(param3, 0, 'f', 2));
  }

  double p1 = params.size() >= 1 ? params[0] : 0.0;
  double p2 = params.size() >= 2 ? params[1] : 0.0;

  if (m_dist->getType() == DistType::Hypergeometric) {
    auto hyper = dynamic_cast<HypergeometricDistribution *>(m_dist);
    if (hyper && params.size() >= 3) {
      hyper->setParameters(p1, p2);
      hyper->setN(static_cast<int>(p1));
      hyper->setK(static_cast<int>(p2));
      hyper->setn(static_cast<int>(params[2]));
    } else {
      m_dist->setParameters(p1, p2);
    }
  } else {
    m_dist->setParameters(p1, p2);
  }

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
