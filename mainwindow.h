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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void setupNormalPlot();
    void setupInitialStyle();
    BaseDistribution* m_currentDist = nullptr;
private slots:
    //统一处理滑块变化的槽函数
    void onParameterChanged();
private:
    void updatePlot();//刷新图像
    double m_currentXViewCenter = 0.0;
    double m_viewWidth = 10.0;
};
#endif // MAINWINDOW_H
