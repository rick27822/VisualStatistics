#include "networkpage.h"
#include "ui_networkpage.h"

NetworkPage::NetworkPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::NetworkPage) {
  ui->setupUi(this);
  setupNodes();
  setupBackButton();
}
void NetworkPage::setupBackButton() {
  connect(ui->btnBack, &QPushButton::clicked, [=]() { emit backToHome(); });
  ui->btnBack->setStyleSheet(
      "QPushButton { background-color: #2D2D2D; color: #00FFF2; "
      "border: 1px solid #00FFF2; border-radius: 5px; padding: 5px 10px; }"
      "QPushButton:hover { background-color: #00FFF2; color: #121212; }");
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

  QString nodeStyle = "QPushButton { border-radius: 40px; background: #1E1E1E; "
                      "border: 1px solid #555; } "
                      "QPushButton:hover { border-color: #00FFF2; }";
  ui->btnNormal->setStyleSheet(nodeStyle);
  ui->btnBinomial->setStyleSheet(nodeStyle);
  ui->btnPoisson->setStyleSheet(nodeStyle);
  ui->btnUniform->setStyleSheet(nodeStyle);
  ui->btnExponential->setStyleSheet(nodeStyle);
  ui->btnStudentT->setStyleSheet(nodeStyle);
  ui->btnChiSquare->setStyleSheet(nodeStyle);
  ui->btnBeta->setStyleSheet(nodeStyle);
  ui->btnGeometric->setStyleSheet(nodeStyle);
  ui->btnHypergeometric->setStyleSheet(nodeStyle);
}
NetworkPage::~NetworkPage() { delete ui; }
