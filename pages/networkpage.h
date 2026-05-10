#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QWidget>
#include "../math/distributionmodel.h"
namespace Ui {
class NetworkPage;
}

class NetworkPage : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkPage(QWidget *parent = nullptr);
    ~NetworkPage();
signals:
    void distSelected(DistType type);
    void backToHome();

private:
    Ui::NetworkPage *ui;
    void setupNodes();
    void setupBackButton();
};

#endif // NETWORKPAGE_H
