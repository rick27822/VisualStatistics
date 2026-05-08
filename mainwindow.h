#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include"math/distributionmodel.h"



QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum class RenderMode { PMF, CDF };
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void setupPlot();
    void setupInitialStyle();
    void plotContinuousDistribution(double start, double end);
    void plotDiscreteDistribution(double start, double end);
    BaseDistribution* m_currentDist = nullptr;
    RenderMode m_mode;
private slots:
    //统一处理滑块变化的槽函数
    void onParameterChanged();
private:
    void updatePlot();//刷新图像
    double m_currentXViewCenter = 0.0;
    double m_viewWidth = 10.0;
};
#endif // MAINWINDOW_H
