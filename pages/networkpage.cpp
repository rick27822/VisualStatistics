#include "networkpage.h"
#include "ui_networkpage.h"
#include <QGradient>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::NetworkPage), hoveredButton(nullptr) {
  ui->setupUi(this);
  setupBackButton();
  setupNodes();
  updateButtonPositions();
}

void NetworkPage::setupBackButton() {
  connect(ui->btnBack, &QPushButton::clicked, [=]() { emit backToHome(); });
  ui->btnBack->setStyleSheet(
      "QPushButton { background-color: #2D2D2D; color: #00FFF2; "
      "border: 1px solid #00FFF2; border-radius: 5px; padding: 5px 10px; }"
      "QPushButton:hover { background-color: #00FFF2; color: #121212; }");
}

void NetworkPage::applyNodeStyle(QPushButton *btn) {
  QString nodeStyle = "QPushButton { "
                      "border-radius: 20px; "
                      "background-color: #1E1E1E; "
                      "border: 2px solid #00FFF2; "
                      "color: #00FFF2; "
                      "font-weight: bold; "
                      "text-align: center; "
                      "padding: 8px 12px; "
                      "min-height: 45px; "
                      "} "
                      "QPushButton:hover { "
                      "border-color: #00FFFF; "
                      "background-color: #2A2A2A; "
                      "}";
  btn->setStyleSheet(nodeStyle);
}

void NetworkPage::setupNodes() {
  connect(ui->btnNormal, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Normal); });

  connect(ui->btnBinomial, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Binomial); });

  connect(ui->btnPoisson, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Poission); });

  connect(ui->btnUniform, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Uniform); });

  connect(ui->btnExponential, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Exponential); });

  connect(ui->btnStudentT, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::StudentT); });

  connect(ui->btnChiSquare, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::ChiSquare); });

  connect(ui->btnBeta, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Beta); });

  connect(ui->btnGeometric, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Geometric); });

  connect(ui->btnHypergeometric, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Hypergeometric); });

  connect(ui->btnFDistribution, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::FDistribution); });

  connect(ui->btnGamma, &QPushButton::clicked,
          [=]() { emit distSelected(DistType::Gamma); });

  QString centerNodeStyle = "QPushButton { "
                            "border-radius: 20px; "
                            "background-color: #1E1E1E; "
                            "border: 3px solid #00FFF2; "
                            "color: #00FFF2; "
                            "font-weight: bold; "
                            "text-align: center; "
                            "padding: 8px 12px; "
                            "min-height: 50px; "
                            "} "
                            "QPushButton:hover { "
                            "border-color: #00FFFF; "
                            "}";
  ui->btnNormal->setStyleSheet(centerNodeStyle);

  applyNodeStyle(ui->btnBinomial);
  applyNodeStyle(ui->btnPoisson);
  applyNodeStyle(ui->btnUniform);
  applyNodeStyle(ui->btnExponential);
  applyNodeStyle(ui->btnStudentT);
  applyNodeStyle(ui->btnChiSquare);
  applyNodeStyle(ui->btnBeta);
  applyNodeStyle(ui->btnGeometric);
  applyNodeStyle(ui->btnHypergeometric);
  applyNodeStyle(ui->btnFDistribution);
  applyNodeStyle(ui->btnGamma);
}

void NetworkPage::updateButtonPositions() {
  int w = width();
  int h = height();

  int centerX = w / 2;
  int centerY = h / 2;
  centerPos = QPoint(centerX, centerY);

  int normalBtnWidth = 150;
  int normalBtnHeight = 55;
  normalBtnRect =
      QRect(centerX - normalBtnWidth / 2, centerY - normalBtnHeight / 2,
            normalBtnWidth, normalBtnHeight);
  ui->btnNormal->setGeometry(normalBtnRect);

  int nodeWidth = 155;
  int nodeHeight = 45;
  int horizontalOffset = 220;
  int verticalOffset = 120;
  int verticalSpacing = 120;

  binomialBtnRect =
      QRect(centerX - horizontalOffset - nodeWidth,
            centerY - verticalOffset - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnBinomial->setGeometry(binomialBtnRect);

  poissonBtnRect = QRect(centerX - horizontalOffset - nodeWidth,
                         centerY - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnPoisson->setGeometry(poissonBtnRect);

  uniformBtnRect =
      QRect(centerX + horizontalOffset,
            centerY - verticalOffset - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnUniform->setGeometry(uniformBtnRect);

  exponentialBtnRect = QRect(centerX + horizontalOffset,
                             centerY - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnExponential->setGeometry(exponentialBtnRect);

  studentTBtnRect =
      QRect(centerX - horizontalOffset - nodeWidth,
            centerY + verticalOffset - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnStudentT->setGeometry(studentTBtnRect);

  chiSquareBtnRect =
      QRect(centerX - horizontalOffset - nodeWidth,
            centerY + verticalOffset + verticalSpacing - nodeHeight / 2,
            nodeWidth, nodeHeight);
  ui->btnChiSquare->setGeometry(chiSquareBtnRect);

  betaBtnRect =
      QRect(centerX + horizontalOffset,
            centerY + verticalOffset - nodeHeight / 2, nodeWidth, nodeHeight);
  ui->btnBeta->setGeometry(betaBtnRect);

  geometricBtnRect =
      QRect(centerX + horizontalOffset,
            centerY + verticalOffset + verticalSpacing - nodeHeight / 2,
            nodeWidth, nodeHeight);
  ui->btnGeometric->setGeometry(geometricBtnRect);

  int hypergeometricWidth = 170;
  hypergeometricBtnRect =
      QRect(centerX - hypergeometricWidth / 2,
            centerY + verticalOffset + verticalSpacing - nodeHeight / 2,
            hypergeometricWidth, nodeHeight);
  ui->btnHypergeometric->setGeometry(hypergeometricBtnRect);

  // 设置 F 分布按钮的位置，放在底部中央
  fDistBtnRect = QRect(centerX - nodeWidth / 2,
                       centerY + verticalOffset + verticalSpacing * 2,
                       nodeWidth, nodeHeight);
  ui->btnFDistribution->setGeometry(fDistBtnRect);

  // 设置伽马分布按钮的位置，放在中间偏下
  gammaBtnRect =
      QRect(centerX - nodeWidth / 2, centerY + verticalOffset - nodeHeight / 2,
            nodeWidth, nodeHeight);
  ui->btnGamma->setGeometry(gammaBtnRect);
}

void NetworkPage::drawBezierCurve(QPainter &painter, const QPoint &start,
                                  const QPoint &end, bool highlighted) {
  QPainterPath path;
  path.moveTo(start);

  QPoint controlPoint;
  if (end.x() < start.x()) {
    controlPoint =
        QPoint((start.x() + end.x()) / 2 - 50, (start.y() + end.y()) / 2);
  } else if (end.x() > start.x()) {
    controlPoint =
        QPoint((start.x() + end.x()) / 2 + 50, (start.y() + end.y()) / 2);
  } else {
    controlPoint =
        QPoint((start.x() + end.x()) / 2, (start.y() + end.y()) / 2 - 50);
  }

  path.quadTo(controlPoint, end);

  int lineWidth = highlighted ? 4 : 2;

  if (highlighted) {
    QLinearGradient gradient(start, end);
    gradient.setColorAt(0, QColor("#00FFF2"));
    gradient.setColorAt(0.5, QColor("#00FFFF"));
    gradient.setColorAt(1, QColor("#00FFF2"));
    painter.setPen(QPen(QBrush(gradient), lineWidth, Qt::SolidLine));
  } else {
    QLinearGradient gradient(start, end);
    gradient.setColorAt(0, QColor("#00FFF2"));
    gradient.setColorAt(1, QColor("#006666"));
    painter.setPen(QPen(QBrush(gradient), lineWidth, Qt::SolidLine));
  }

  painter.drawPath(path);
}

void NetworkPage::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  QPoint center = normalBtnRect.center();

  QVector<QPair<QPushButton *, QRect>> nodes = {
      {ui->btnBinomial, binomialBtnRect},
      {ui->btnPoisson, poissonBtnRect},
      {ui->btnUniform, uniformBtnRect},
      {ui->btnExponential, exponentialBtnRect},
      {ui->btnStudentT, studentTBtnRect},
      {ui->btnChiSquare, chiSquareBtnRect},
      {ui->btnBeta, betaBtnRect},
      {ui->btnGeometric, geometricBtnRect},
      {ui->btnHypergeometric, hypergeometricBtnRect},
      {ui->btnFDistribution, fDistBtnRect},
      {ui->btnGamma, gammaBtnRect}};

  for (const auto &pair : nodes) {
    QPoint nodeCenter = pair.second.center();
    bool highlighted = (hoveredButton == pair.first);
    drawBezierCurve(painter, center, nodeCenter, highlighted);
  }

  QRadialGradient centerGradient(center, 10);
  centerGradient.setColorAt(0, QColor("#00FFFF"));
  centerGradient.setColorAt(1, QColor("#00FFF2"));
  painter.setBrush(QBrush(centerGradient));
  painter.setPen(Qt::NoPen);
  painter.drawEllipse(center, 6, 6);

  for (const auto &pair : nodes) {
    QPoint nodeCenter = pair.second.center();
    bool highlighted = (hoveredButton == pair.first);

    if (highlighted) {
      QRadialGradient nodeGradient(nodeCenter, 5);
      nodeGradient.setColorAt(0, QColor("#00FFFF"));
      nodeGradient.setColorAt(1, QColor("#00FFF2"));
      painter.setBrush(QBrush(nodeGradient));
      painter.drawEllipse(nodeCenter, 5, 5);
    } else {
      painter.setBrush(QColor("#00FFF2"));
      painter.drawEllipse(nodeCenter, 4, 4);
    }
  }
}

void NetworkPage::resizeEvent(QResizeEvent *) {
  updateButtonPositions();
  update();
}

void NetworkPage::mouseMoveEvent(QMouseEvent *event) {
  QVector<QPushButton *> buttons = {
      ui->btnBinomial,      ui->btnPoisson,   ui->btnUniform,
      ui->btnExponential,   ui->btnStudentT,  ui->btnChiSquare,
      ui->btnBeta,          ui->btnGeometric, ui->btnHypergeometric,
      ui->btnFDistribution, ui->btnGamma,     ui->btnNormal};

  QPushButton *newHovered = nullptr;
  for (QPushButton *btn : buttons) {
    if (btn->geometry().contains(event->pos())) {
      newHovered = btn;
      break;
    }
  }

  if (newHovered != hoveredButton) {
    hoveredButton = newHovered;
    update();
  }
}

void NetworkPage::leaveEvent(QEvent *) {
  hoveredButton = nullptr;
  update();
}

NetworkPage::~NetworkPage() { delete ui; }
