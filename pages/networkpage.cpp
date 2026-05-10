#include "networkpage.h"
#include "ui_networkpage.h"

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::NetworkPage)
{
    ui->setupUi(this);
    setupNodes();
}
void NetworkPage::setupNodes() {
    connect(ui->btnNormal, &QPushButton::clicked, [=](){
        emit distSelected(DistType::Normal);
    });

    connect(ui->btnBinomial, &QPushButton::clicked, [=](){
        emit distSelected(DistType::Binomial);
    });

    // 样式美化
    QString nodeStyle = "QPushButton { border-radius: 40px; background: #1E1E1E; border: 1px solid #555; } ""QPushButton:hover { border-color: #00FFF2; }";
    ui->btnNormal->setStyleSheet(nodeStyle);
    ui->btnBinomial->setStyleSheet(nodeStyle);
}
NetworkPage::~NetworkPage()
{
    delete ui;
}
