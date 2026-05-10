#include "welcomepage.h"
#include "ui_welcomepage.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomePage)
{
    ui->setupUi(this);

    ui->btnStart->setText("EXPLORE DISTRIBUTIONS");
    ui->btnStart->setStyleSheet(
        "QPushButton { "
        "  background-color: transparent; border: 2px solid #00FFF2; "
        "  color: #00FFF2; padding: 15px 30px; font-size: 18px; border-radius: 5px;"
        "}"
        "QPushButton:hover { background-color: rgba(0, 255, 242, 0.1); }"
        );

    connect(ui->btnStart, &QPushButton::clicked, this, &WelcomePage::startExploring);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}
