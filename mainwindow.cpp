#include "mainwindow.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    //初始化容器
    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    //实例化页面
    m_welcomePage = new WelcomePage(this);
    m_networkPage = new NetworkPage(this);
    m_detailPage  = new DetailPage(this);

    //加入堆栈
    m_stack->addWidget(m_welcomePage); // Index 0
    m_stack->addWidget(m_networkPage); // Index 1
    m_stack->addWidget(m_detailPage);  // Index 2

    setupConnections();
    applyGlobalStyle();
}
void MainWindow::setupConnections() {
    // 首页 -> 关系网
    connect(m_welcomePage, &WelcomePage::startExploring, [=](){
        m_stack->setCurrentIndex(1);
    });

    // 关系网 -> 详情页
    connect(m_networkPage, &NetworkPage::distSelected, [=](DistType type){
        m_detailPage->setDistribution(DistFactory::create(type));
        m_stack->setCurrentIndex(2);
    });

    // 详情页 -> 关系网 (返回)
    connect(m_detailPage, &DetailPage::backRequested, [=](){
        m_stack->setCurrentIndex(1);
    });

    // 关系网 -> 首页 (返回)
    connect(m_networkPage, &NetworkPage::backToHome, [=](){
        m_stack->setCurrentIndex(0);
    });
}
void MainWindow::applyGlobalStyle() {
    // 3b1b 核心色调：深灰背景 + 青色高亮
    this->setStyleSheet(
        "QMainWindow { background-color: #121212; }"
        "QWidget { color: #E0E0E0; font-family: 'Arial'; }"
        );
}
MainWindow::~MainWindow()
{
    delete ui;
}
