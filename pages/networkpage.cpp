#include "networkpage.h"
#include "ui_networkpage.h"
#include <QGradient>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QResizeEvent>
#include <QShowEvent>

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::NetworkPage), hoveredButton(nullptr) {
  ui->setupUi(this);
  setupBackButton();
  setupNodes();
  updateButtonPositions();
  setupRelations();

  animationTimer = new QTimer(this);
  connect(animationTimer, &QTimer::timeout, this,
          &NetworkPage::updateParticles);
  animationTimer->start(50);
}

void NetworkPage::showEvent(QShowEvent *event) {
  QWidget::showEvent(event);
  initParticles();
}

void NetworkPage::setupBackButton() {
  connect(ui->btnBack, &QPushButton::clicked, [=]() { emit backToHome(); });
  ui->btnBack->setStyleSheet(
      "QPushButton { background-color: #2D2D2D; color: #00FFF2; border: 1px "
      "solid #00FFF2; border-radius: 5px; padding: 5px 10px; } "
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
                            "background-color: #2A2A2A; "
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

  binomialBtnRect = QRect(centerX - 380, centerY - 100, nodeWidth, nodeHeight);
  ui->btnBinomial->setGeometry(binomialBtnRect);

  poissonBtnRect = QRect(centerX - 120, centerY - 300, nodeWidth, nodeHeight);
  ui->btnPoisson->setGeometry(poissonBtnRect);

  uniformBtnRect = QRect(centerX + 300, centerY - 280, nodeWidth, nodeHeight);
  ui->btnUniform->setGeometry(uniformBtnRect);

  exponentialBtnRect =
      QRect(centerX + 400, centerY - 80, nodeWidth, nodeHeight);
  ui->btnExponential->setGeometry(exponentialBtnRect);

  studentTBtnRect = QRect(centerX - 380, centerY + 220, nodeWidth, nodeHeight);
  ui->btnStudentT->setGeometry(studentTBtnRect);

  chiSquareBtnRect = QRect(centerX - 450, centerY + 50, nodeWidth, nodeHeight);
  ui->btnChiSquare->setGeometry(chiSquareBtnRect);

  betaBtnRect = QRect(centerX + 380, centerY + 250, nodeWidth, nodeHeight);
  ui->btnBeta->setGeometry(betaBtnRect);

  geometricBtnRect = QRect(centerX + 80, centerY + 340, nodeWidth, nodeHeight);
  ui->btnGeometric->setGeometry(geometricBtnRect);

  int hypergeometricWidth = 170;
  hypergeometricBtnRect =
      QRect(centerX + 220, centerY - 380, hypergeometricWidth, nodeHeight);
  ui->btnHypergeometric->setGeometry(hypergeometricBtnRect);

  fDistBtnRect = QRect(centerX - 250, centerY + 380, nodeWidth, nodeHeight);
  ui->btnFDistribution->setGeometry(fDistBtnRect);

  gammaBtnRect = QRect(centerX + 250, centerY + 120, nodeWidth, nodeHeight);
  ui->btnGamma->setGeometry(gammaBtnRect);
}

void NetworkPage::setupRelations() {
  relations.clear();

  // 二项分布 → 泊松分布
  relations.append({&binomialBtnRect, &poissonBtnRect, "n→∞, p→0"});

  // 二项分布 → 正态分布
  relations.append(
      {&binomialBtnRect, &normalBtnRect, "n足够大 (De Moivre-Laplace)"});

  // 超几何分布 → 二项分布
  relations.append(
      {&hypergeometricBtnRect, &binomialBtnRect, "总体N极大，抽样比例极小"});

  // 指数分布 → 伽马分布
  relations.append(
      {&exponentialBtnRect, &gammaBtnRect, "n个独立指数分布变量之和"});

  // 指数分布 → 泊松分布
  relations.append({&exponentialBtnRect, &poissonBtnRect,
                    "指数分布是泊松过程中两次事件的时间间隔"});

  // t分布 → F分布
  relations.append({&studentTBtnRect, &fDistBtnRect, "t²(n)=F(1,n)"});

  // 正态分布 → 卡方分布
  relations.append({&normalBtnRect, &chiSquareBtnRect, "Z₁²+...+Zₖ²~χ²(k)"});

  // 伽马分布 → 卡方分布
  relations.append(
      {&gammaBtnRect, &chiSquareBtnRect, "Gamma(α=n/2,β=1/2)=χ²(n)"});

  // 伽马分布 → Beta分布
  relations.append({&gammaBtnRect, &betaBtnRect, "X/(X+Y)~Beta(α,β)"});
}

void NetworkPage::drawBezierCurve(QPainter &painter, const QPoint &start,
                                  const QPoint &end, const QString &text,
                                  bool highlighted) {
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

void NetworkPage::drawRelationText(QPainter &painter, const QPoint &start,
                                   const QPoint &end, const QString &text) {
  if (text.isEmpty()) {
    return;
  }

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

  qreal t = 0.5;
  qreal oneMinusT = 1.0 - t;
  QPoint midPoint = oneMinusT * oneMinusT * start +
                    2 * oneMinusT * t * controlPoint + t * t * end;

  int fontSize = qMax(9, 12 - text.length() / 4);
  QFont font("Arial", fontSize);
  painter.setFont(font);
  QFontMetrics fm(font);
  QRect textRect = fm.boundingRect(text);
  textRect.moveCenter(midPoint);
  textRect.adjust(-6, -3, 6, 3);

  QRect windowRect = painter.window();
  textRect.moveLeft(qMax(textRect.left(), 10));
  textRect.moveTop(qMax(textRect.top(), 10));
  textRect.moveRight(qMin(textRect.right(), windowRect.width() - 10));
  textRect.moveBottom(qMin(textRect.bottom(), windowRect.height() - 10));

  painter.setBrush(QColor("#1E1E1E"));
  painter.setPen(QPen(QColor("#00FFF2"), 1));
  painter.drawRoundedRect(textRect, 4, 4);

  painter.setPen(QColor("#00FFF2"));
  painter.drawText(textRect, Qt::AlignCenter, text);
}

void NetworkPage::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  drawParticleGrid(painter);

  // 第一步：绘制所有分布关系曲线（不带文字）
  for (const auto &relation : relations) {
    QPoint sourceCenter = relation.sourceRect->center();
    QPoint targetCenter = relation.targetRect->center();

    bool highlighted = false;
    QVector<QPushButton *> allButtons = {
        ui->btnBinomial,       ui->btnPoisson,      ui->btnNormal,
        ui->btnHypergeometric, ui->btnExponential,  ui->btnGamma,
        ui->btnStudentT,       ui->btnFDistribution};

    for (QPushButton *btn : allButtons) {
      if (hoveredButton == btn) {
        if ((btn->geometry() == *relation.sourceRect) ||
            (btn->geometry() == *relation.targetRect)) {
          highlighted = true;
          break;
        }
      }
    }

    drawBezierCurve(painter, sourceCenter, targetCenter, "", highlighted);
  }

  // 第二步：绘制所有关系文字标签（在曲线上方）
  for (const auto &relation : relations) {
    QPoint sourceCenter = relation.sourceRect->center();
    QPoint targetCenter = relation.targetRect->center();
    drawRelationText(painter, sourceCenter, targetCenter, relation.condition);
  }

  // 第三步：绘制节点圆点（在最上方）
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
      {ui->btnGamma, gammaBtnRect},
      {ui->btnNormal, normalBtnRect}};

  for (const auto &pair : nodes) {
    QPoint nodeCenter = pair.second.center();
    bool highlighted = (hoveredButton == pair.first);

    if (highlighted) {
      QRadialGradient nodeGradient(nodeCenter, 6);
      nodeGradient.setColorAt(0, QColor("#00FFFF"));
      nodeGradient.setColorAt(1, QColor("#00FFF2"));
      painter.setBrush(QBrush(nodeGradient));
      painter.drawEllipse(nodeCenter, 6, 6);
    } else {
      painter.setBrush(QColor("#00FFF2"));
      painter.drawEllipse(nodeCenter, 4, 4);
    }
  }
}

void NetworkPage::resizeEvent(QResizeEvent *) {
  updateButtonPositions();
  setupRelations();
  initParticles();
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

void NetworkPage::initParticles() {
  int numParticles = 120;
  particles.clear();
  particleConnections.clear();

  auto *rng = QRandomGenerator::global();

  for (int i = 0; i < numParticles; ++i) {
    Particle p;
    p.pos = QPointF(rng->bounded(width()), rng->bounded(height()));
    p.velocity = QPointF((rng->bounded(100) - 50) / 200.0,
                         (rng->bounded(100) - 50) / 200.0);
    p.size = 2.0 + rng->bounded(15) / 10.0;
    p.opacity = 0.2 + rng->bounded(40) / 100.0;
    particles.append(p);
  }

  int numConnections = 100;
  for (int i = 0; i < numConnections; ++i) {
    int p1 = rng->bounded(numParticles);
    int p2 = rng->bounded(numParticles);
    if (p1 != p2) {
      ParticleConnection conn;
      conn.particle1 = p1;
      conn.particle2 = p2;
      conn.opacity = 0.1 + rng->bounded(30) / 100.0;
      particleConnections.append(conn);
    }
  }
}

void NetworkPage::updateParticles() {
  for (int i = 0; i < particles.size(); ++i) {
    Particle &p = particles[i];
    p.pos += p.velocity;

    if (p.pos.x() < 0)
      p.pos.setX(width());
    if (p.pos.x() > width())
      p.pos.setX(0);
    if (p.pos.y() < 0)
      p.pos.setY(height());
    if (p.pos.y() > height())
      p.pos.setY(0);
  }
  update();
}

void NetworkPage::drawParticleGrid(QPainter &painter) {
  for (const auto &conn : particleConnections) {
    const Particle &p1 = particles[conn.particle1];
    const Particle &p2 = particles[conn.particle2];
    QColor lineColor(0, 255, 242);
    lineColor.setAlphaF(conn.opacity);
    painter.setPen(QPen(lineColor, 0.8));
    painter.drawLine(p1.pos, p2.pos);
  }

  for (const auto &p : particles) {
    QColor particleColor(0, 255, 242);
    particleColor.setAlphaF(p.opacity);
    painter.setBrush(particleColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(p.pos, p.size, p.size);
  }
}

NetworkPage::~NetworkPage() { delete ui; }
