#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
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

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *) override;

private:
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

    void setupNodes();
    void setupBackButton();
    void updateButtonPositions();
    void applyNodeStyle(QPushButton *btn);
    void drawBezierCurve(QPainter &painter, const QPoint &start, const QPoint &end, bool highlighted);
};

#endif // NETWORKPAGE_H
