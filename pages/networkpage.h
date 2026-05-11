#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include "../math/distributionmodel.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

namespace Ui {
class NetworkPage;
}

struct DistributionRelation {
  QRect *sourceRect;
  QRect *targetRect;
  QString condition;
};

class NetworkPage : public QWidget {
  Q_OBJECT

public:
  explicit NetworkPage(QWidget *parent = nullptr);
  ~NetworkPage();

signals:
  void distSelected(DistType type);
  void backToHome();

protected:
  void paintEvent(QPaintEvent *) override;
  void resizeEvent(QResizeEvent *) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void leaveEvent(QEvent *) override;
  void showEvent(QShowEvent *event) override;

private:
  struct Particle {
    QPointF pos;
    QPointF velocity;
    qreal size;
    qreal opacity;
  };

  struct ParticleConnection {
    int particle1;
    int particle2;
    qreal opacity;
  };

  Ui::NetworkPage *ui;
  QPushButton *hoveredButton;
  QPoint centerPos;
  QRect normalBtnRect;
  QRect binomialBtnRect;
  QRect poissonBtnRect;
  QRect uniformBtnRect;
  QRect exponentialBtnRect;
  QRect studentTBtnRect;
  QRect chiSquareBtnRect;
  QRect betaBtnRect;
  QRect geometricBtnRect;
  QRect hypergeometricBtnRect;
  QRect fDistBtnRect;
  QRect gammaBtnRect;
  QVector<DistributionRelation> relations;

  QVector<Particle> particles;
  QVector<ParticleConnection> particleConnections;
  QTimer *animationTimer;

  void setupNodes();
  void setupBackButton();
  void updateButtonPositions();
  void applyNodeStyle(QPushButton *btn);
  void setupRelations();
  void drawBezierCurve(QPainter &painter, const QPoint &start,
                       const QPoint &end, const QString &text,
                       bool highlighted);
  void drawRelationText(QPainter &painter, const QPoint &start,
                        const QPoint &end, const QString &text);
  void initParticles();
  void updateParticles();
  void drawParticleGrid(QPainter &painter);
};

#endif // NETWORKPAGE_H
