#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include<cmath>
#include<vector>
#include<QPair>
#include<QString>


using namespace std;

enum class DistType {Normal,Binomial,Poission};

//定义一个基类
class BaseDistribution{
public:
    virtual ~BaseDistribution() {}

    virtual double calculate(double x) const =0;//计算概率密度

    virtual QPair<double,double> getSuggestedRange() const = 0;//获取建议的范围
    //获取参数名称
    virtual QString getParam1Name() const = 0;
    virtual QString getParam2Name() const = 0;

    virtual void setParameters(double p1,double p2)=0;//更新参数

};

class NormalDistribution: public BaseDistribution{
private:
    double m_mu;
    double m_sigma;
public:
    NormalDistribution(double mu=0.0,double sigma =1.0) :m_mu(mu),m_sigma(sigma){}

    double calculate(double x) const override;
    QPair<double, double> getSuggestedRange() const override;
    QString getParam1Name() const override { return "期望 (μ)"; }
    QString getParam2Name() const override { return "标准差 (σ)"; }
    void setParameters(double p1, double p2) override { m_mu = p1; m_sigma = p2; }
};

#endif // DISTRIBUTIONMODEL_H
