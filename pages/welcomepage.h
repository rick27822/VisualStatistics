#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QPainter>
#include <QRandomGenerator>
#include <QTimer>
#include <QVector>
#include <QWidget>

struct Particle {
  qreal x;
  qreal y;
  qreal speedY;
  qreal opacity;
  bool settled;
  int binIndex;
};

class WelcomePage : public QWidget {
  Q_OBJECT

public:
  explicit WelcomePage(QWidget *parent = nullptr);
  ~WelcomePage();

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

signals:
  void startExploring();

private slots:
  void updateParticles();

private:
  QTimer *m_timer;
  QVector<Particle> m_particles;
  QVector<int> m_bins;
  int m_binCount;
  qreal m_binWidth;
  qreal m_maxCurveHeight;
  QRandomGenerator m_random;
  qreal m_oldWidth;
  qreal m_oldHeight;

  void initUI();
  void spawnParticle();
  void recalculateParticlePositions();
};

#endif // WELCOMEPAGE_H