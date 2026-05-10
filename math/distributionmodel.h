#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include<QPair>
#include<QString>


using namespace std;

enum class DistType {Normal,Binomial,Poission};
enum class DistributionCategory {Continuous,Discrete};

//定义一个基类
class BaseDistribution{
public:
    virtual ~BaseDistribution() {}
    virtual DistType getType() const = 0;//获取分布类型
    virtual DistributionCategory getCategory() const = 0;//获取分布类别
    virtual double calculate(double x) const =0;//计算概率密度
    virtual double calculateCDF(double x) const = 0;
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
    double calculateCDF(double x) const override;
    double calculate(double x) const override;
    QPair<double, double> getSuggestedRange() const override;
    QString getParam1Name() const override { return "期望 (μ)"; }
    QString getParam2Name() const override { return "标准差 (σ)"; }
    void setParameters(double p1, double p2) override { m_mu = p1; m_sigma = p2; }
    DistType getType() const override { return DistType::Normal; }
    DistributionCategory getCategory() const override { return DistributionCategory::Continuous; }
};
class BinomialDistribution: public BaseDistribution{
private:
    int n;
    double p;
public:
    BinomialDistribution(int n=5,double p=0.5) : n(n),p(p){}
    double calculate(double x) const override;
    double calculateCDF(double x) const override;
    QPair<double, double> getSuggestedRange() const override;
    QString getParam1Name() const override { return "实验次数 (n)"; }
    QString getParam2Name() const override { return "概率 (p)"; }
    DistType getType() const override { return DistType::Binomial; }
    DistributionCategory getCategory() const override { return DistributionCategory::Discrete; }
    void setParameters(double p1, double p2) override {
        n = std::max(1, (int)p1); // n 必须是正整数
        p = std::clamp(p2, 0.0, 1.0); // p 必须在 0 到 1 之间
    }
};

struct DistFactory {
    static BaseDistribution* create(DistType type) {
        switch (type) {
            case DistType::Normal:    return new NormalDistribution(0.0, 1.0);
            case DistType::Binomial:  return new BinomialDistribution(10, 0.5);
            default:                  return new NormalDistribution(0.0, 1.0);
        }
    }
};

#endif // DISTRIBUTIONMODEL_H
