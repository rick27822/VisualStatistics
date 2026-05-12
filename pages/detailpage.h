#ifndef DETAILPAGE_H
#define DETAILPAGE_H

#include "../math/distributionmodel.h"
#include "../qcustomplot.h"
#include <QWidget>
#include <QWebEngineView>
#include <memory>

namespace Ui {
class DetailPage;
}

class DetailPage : public QWidget {
  Q_OBJECT

public:
  enum class RenderMode { PMF, CDF };
  explicit DetailPage(QWidget *parent = nullptr);
  ~DetailPage();
  void setDistribution(std::unique_ptr<BaseDistribution> dist);
signals:
  void backRequested();
  void jumpToDistribution(DistType type);

private slots:
  void onParameterChanged();
  void toggleRenderMode();
  void onLinkClicked(const QUrl &url);

private:
  Ui::DetailPage *ui;
  std::unique_ptr<BaseDistribution> m_dist;
  RenderMode m_mode = RenderMode::PMF;
  void setupInitialStyle();
  void setupSliderRanges();
  void updatePlot();
  void updateDescription();
  void updateRelatedDistributions();
  void plotContinuousDistribution(double start, double end);
  void plotDiscreteDistribution(double start, double end);
};

#endif // DETAILPAGE_H
