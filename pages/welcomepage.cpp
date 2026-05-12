#include "welcomepage.h"
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QRadialGradient>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <cmath>

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent), m_timer(new QTimer(this)), m_binCount(60),
      m_maxCurveHeight(0), m_oldWidth(0), m_oldHeight(0) {
  m_bins.resize(m_binCount, 0);

  initUI();

  connect(m_timer, &QTimer::timeout, this, &WelcomePage::updateParticles);
  m_timer->start(16);
}

WelcomePage::~WelcomePage() { delete m_timer; }

void WelcomePage::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);

  if (m_oldWidth > 0 && m_oldHeight > 0) {
    recalculateParticlePositions();
  }

  m_oldWidth = width();
  m_oldHeight = height();
  m_binWidth = static_cast<qreal>(width()) / m_binCount;
  m_maxCurveHeight = height() * 0.6;
}

void WelcomePage::recalculateParticlePositions() {
  qreal settleY = height() - 50;
  qreal oldSettleY = m_oldHeight - 50;

  for (int i = 0; i < m_binCount; ++i) {
    m_bins[i] = 0;
  }

  qreal maxHeight = 0;
  for (const auto &p : m_particles) {
    if (p.settled) {
      qreal h = oldSettleY - p.y;
      if (h > maxHeight) {
        maxHeight = h;
      }
    }
  }

  qreal newMaxHeight = m_maxCurveHeight * 0.9;

  for (auto &p : m_particles) {
    qreal oldCenterX = m_oldWidth / 2.0;
    qreal newCenterX = width() / 2.0;
    qreal relativeX = (p.x - oldCenterX) / (m_oldWidth / 2.0);
    p.x = newCenterX + relativeX * (width() / 2.0);

    if (p.settled) {
      qreal oldRelativeHeight =
          maxHeight > 0 ? (oldSettleY - p.y) / maxHeight : 0;
      p.y = settleY - oldRelativeHeight * newMaxHeight;

      p.binIndex = static_cast<int>((p.x / width()) * m_binCount);
      p.binIndex = qBound(0, p.binIndex, m_binCount - 1);

      m_bins[p.binIndex]++;
    } else {
      qreal relativeY = p.y / m_oldHeight;
      p.y = relativeY * height();
    }
  }
}

void WelcomePage::initUI() {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(50, 50, 50, 50);
  layout->setSpacing(30);
  layout->setAlignment(Qt::AlignCenter);

  QLabel *titleLabel = new QLabel("VISUAL STATISTICS", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setStyleSheet("QLabel { "
                            "  font-size: 32px; "
                            "  font-weight: bold; "
                            "  color: #FFFFFF; "
                            "  letter-spacing: 8px; "
                            "  background-color: rgba(0, 0, 0, 0.8); "
                            "  padding: 20px 40px; "
                            "  border-radius: 10px; "
                            "  border: 1px solid rgba(0, 255, 242, 0.5); "
                            "}");
  layout->addWidget(titleLabel);

  QPushButton *exploreBtn = new QPushButton("EXPLORE", this);
  exploreBtn->setStyleSheet("QPushButton { "
                            "  background-color: rgba(0, 0, 0, 0.9); "
                            "  border: 2px solid #00FFF2; "
                            "  color: #00FFF2; "
                            "  padding: 15px 50px; "
                            "  font-size: 18px; "
                            "  font-weight: bold; "
                            "  border-radius: 8px; "
                            "  min-width: 200px; "
                            "}"
                            "QPushButton:hover { "
                            "  background-color: rgba(0, 255, 242, 0.2); "
                            "}");
  layout->addWidget(exploreBtn, 0, Qt::AlignCenter);

  connect(exploreBtn, &QPushButton::clicked, this,
          &WelcomePage::startExploring);

  QTimer::singleShot(100, this, [=]() {
    QGraphicsDropShadowEffect *titleEffect =
        new QGraphicsDropShadowEffect(this);
    titleEffect->setColor(QColor(0, 255, 242));
    titleEffect->setBlurRadius(20);
    titleEffect->setOffset(0, 0);
    titleLabel->setGraphicsEffect(titleEffect);

    QGraphicsDropShadowEffect *btnEffect = new QGraphicsDropShadowEffect(this);
    btnEffect->setColor(QColor(0, 255, 242));
    btnEffect->setBlurRadius(15);
    btnEffect->setOffset(0, 0);
    exploreBtn->setGraphicsEffect(btnEffect);
  });
}

void WelcomePage::spawnParticle() {
  Particle p;
  qreal mean = width() / 2.0;
  qreal stdDev = width() / 5.0;

  qreal u1 = m_random.generateDouble();
  qreal u2 = m_random.generateDouble();
  qreal z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
  p.x = mean + z * stdDev;
  p.x = qBound(10.0, p.x, width() - 10.0);

  p.y = -10;
  p.speedY = 3.0 + m_random.generateDouble() * 3.0;
  p.opacity = 0.12 + m_random.generateDouble() * 0.15;
  p.settled = false;
  p.binIndex = static_cast<int>((p.x / width()) * m_binCount);
  p.binIndex = qBound(0, p.binIndex, m_binCount - 1);

  m_particles.append(p);
}

void WelcomePage::updateParticles() {
  for (int i = 0; i < 6; ++i) {
    if (m_particles.size() < 6000) {
      spawnParticle();
    }
  }

  qreal settleY = height() - 50;
  m_binWidth = static_cast<qreal>(width()) / m_binCount;

  qreal safeHeight = height() * 0.6;
  qreal maxBins = safeHeight / 4.0;

  qreal mean = width() / 2.0;
  qreal stdDev = width() / 5.0;

  for (int i = 0; i < m_particles.size(); ++i) {
    Particle &p = m_particles[i];
    if (!p.settled) {
      p.y += p.speedY;

      int bin = static_cast<int>(p.x / m_binWidth);
      bin = qBound(0, bin, m_binCount - 1);

      qreal binCenter = (bin + 0.5) * m_binWidth;
      qreal z = (binCenter - mean) / stdDev;
      qreal gaussian = exp(-0.5 * z * z);
      qreal effectiveMaxBins = maxBins * gaussian;

      if (m_bins[bin] >= effectiveMaxBins) {
        m_particles.remove(i);
        i--;
        continue;
      }

      qreal settleHeight = settleY - m_bins[bin] * 4.0;
      if (p.y >= settleHeight) {
        p.y = settleHeight;
        p.settled = true;
        p.binIndex = bin;
        m_bins[bin]++;
      }
    }
  }

  update();
}

void WelcomePage::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event)
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), QColor(12, 12, 12));

  for (const Particle &p : m_particles) {
    QColor color(0, 255, 242);
    color.setAlphaF(p.opacity);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(QPointF(p.x, p.y), 2, 2);
  }

  qreal settleY = height() - 50;

  QRadialGradient radialGrad(width() / 2, settleY - m_maxCurveHeight * 0.3,
                             width() * 0.6);
  radialGrad.setColorAt(0, QColor(0, 255, 242, 50));
  radialGrad.setColorAt(0.4, QColor(0, 255, 242, 25));
  radialGrad.setColorAt(0.8, QColor(0, 255, 242, 8));
  radialGrad.setColorAt(1, QColor(0, 255, 242, 0));
  painter.setBrush(radialGrad);
  painter.setPen(Qt::NoPen);

  QPainterPath path;
  path.moveTo(0, settleY);

  qreal maxHeight = 0;
  for (int i = 0; i < m_binCount; ++i) {
    qreal h = qMin(m_bins[i] * 4.0, m_maxCurveHeight);
    if (h > maxHeight)
      maxHeight = h;
  }

  if (maxHeight > 0) {
    qreal mean = width() / 2.0;
    qreal stdDev = width() / 5.0;

    for (qreal x = 0; x <= width(); x += 2) {
      qreal z = (x - mean) / stdDev;
      qreal gaussian = exp(-0.5 * z * z);
      qreal h = gaussian * maxHeight * 1.2;
      path.lineTo(x, settleY - h);
    }
  }

  path.lineTo(width(), settleY);
  path.closeSubpath();
  painter.drawPath(path);

  QLinearGradient glowGrad(width() / 2, settleY - m_maxCurveHeight, width() / 2,
                           settleY);
  glowGrad.setColorAt(0, QColor(0, 255, 242, 30));
  glowGrad.setColorAt(0.5, QColor(0, 255, 242, 15));
  glowGrad.setColorAt(1, QColor(0, 255, 242, 3));
  painter.setBrush(glowGrad);

  qreal safeHeight = height() * 0.6;
  qreal currentMaxHeight = 0;
  for (int i = 0; i < m_binCount; ++i) {
    qreal h = m_bins[i] * 4.0;
    if (h > currentMaxHeight)
      currentMaxHeight = h;
  }

  if (currentMaxHeight > 0) {
    qreal mean = width() / 2.0;
    qreal stdDev = width() / 5.0;

    for (int i = 0; i < m_binCount; ++i) {
      qreal x = i * m_binWidth;
      qreal binCenter = (i + 0.5) * m_binWidth;
      qreal z = (binCenter - mean) / stdDev;
      qreal gaussian = exp(-0.5 * z * z);

      qreal h = m_bins[i] * 4.0;

      qreal effectiveMaxHeight = safeHeight * gaussian;
      h = qMin(h, effectiveMaxHeight);

      if (h > 0) {
        QPainterPath binPath;
        binPath.addRoundedRect(QRectF(x + 1, settleY - h, m_binWidth - 2, h), 2,
                               2);
        painter.drawPath(binPath);
      }
    }
  }

  painter.setBrush(QColor(0, 255, 242, 15));
  for (const Particle &p : m_particles) {
    if (p.settled) {
      painter.drawEllipse(QPointF(p.x, p.y), 2, 2);
    }
  }
}