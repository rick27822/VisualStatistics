#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QStackedWidget>
#include "pages/welcomepage.h"
#include "pages/networkpage.h"
#include "pages/detailpage.h"



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
    QStackedWidget *m_stack;

    // 页面指针
    WelcomePage *m_welcomePage;
    NetworkPage *m_networkPage;
    DetailPage  *m_detailPage;

    void setupConnections();
    void applyGlobalStyle();
};
#endif // MAINWINDOW_H
