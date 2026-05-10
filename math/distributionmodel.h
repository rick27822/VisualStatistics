#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include <QPair>
#include <QString>

using namespace std;

enum class DistType {
  Normal,
  Binomial,
  Poission,
  Uniform,
  Exponential,
  StudentT,
  ChiSquare,
  Beta,
  Geometric,
  Hypergeometric
};
enum class DistributionCategory { Continuous, Discrete };

// 定义一个基类
class BaseDistribution {
public:
  virtual ~BaseDistribution() {}
  virtual DistType getType() const = 0;                 // 获取分布类型
  virtual DistributionCategory getCategory() const = 0; // 获取分布类别
  virtual double calculate(double x) const = 0;         // 计算概率密度
  virtual double calculateCDF(double x) const = 0;
  virtual QPair<double, double> getSuggestedRange() const = 0; // 获取建议的范围
  virtual QString getName() const = 0;                         // 获取分布名称
  // 获取参数名称
  virtual QString getParam1Name() const = 0;
  virtual QString getParam2Name() const = 0;

  virtual void setParameters(double p1, double p2) = 0; // 更新参数
};

class NormalDistribution : public BaseDistribution {
private:
  double m_mu;
  double m_sigma;

public:
  NormalDistribution(double mu = 0.0, double sigma = 1.0)
      : m_mu(mu), m_sigma(sigma) {}
  double calculateCDF(double x) const override;
  double calculate(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "期望 (μ)"; }
  QString getParam2Name() const override { return "标准差 (σ)"; }
  QString getName() const override { return "正态分布"; }
  void setParameters(double p1, double p2) override {
    m_mu = p1;
    m_sigma = p2;
  }
  DistType getType() const override { return DistType::Normal; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
};
class BinomialDistribution : public BaseDistribution {
private:
  int n;
  double p;

public:
  BinomialDistribution(int n = 5, double p = 0.5) : n(n), p(p) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "实验次数 (n)"; }
  QString getParam2Name() const override { return "概率 (p)"; }
  QString getName() const override { return "二项分布"; }
  DistType getType() const override { return DistType::Binomial; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Discrete;
  }
  void setParameters(double p1, double p2) override {
    n = std::max(1, (int)p1);     // n 必须是正整数
    p = std::clamp(p2, 0.0, 1.0); // p 必须在 0 到 1 之间
  }
};

class PoissonDistribution : public BaseDistribution {
private:
  double m_lambda;

public:
  PoissonDistribution(double lambda = 5.0) : m_lambda(lambda) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "均值 (λ)"; }
  QString getParam2Name() const override { return ""; }
  QString getName() const override { return "泊松分布"; }
  DistType getType() const override { return DistType::Poission; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Discrete;
  }
  void setParameters(double p1, double p2) override {
    Q_UNUSED(p2);
    m_lambda = std::max(0.01, p1); // λ 必须大于 0
  }
};

class UniformDistribution : public BaseDistribution {
private:
  double m_a;
  double m_b;

public:
  UniformDistribution(double a = 0.0, double b = 1.0) : m_a(a), m_b(b) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "下限 (a)"; }
  QString getParam2Name() const override { return "上限 (b)"; }
  QString getName() const override { return "均匀分布"; }
  DistType getType() const override { return DistType::Uniform; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    m_a = p1;
    m_b = std::max(p1 + 0.01, p2);
  }
};

class ExponentialDistribution : public BaseDistribution {
private:
  double m_lambda;

public:
  ExponentialDistribution(double lambda = 1.0) : m_lambda(lambda) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "率参数 (λ)"; }
  QString getParam2Name() const override { return ""; }
  QString getName() const override { return "指数分布"; }
  DistType getType() const override { return DistType::Exponential; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    Q_UNUSED(p2);
    m_lambda = std::max(0.01, p1); // λ 必须大于 0
  }
};

class StudentTDistribution : public BaseDistribution {
private:
  double m_df; // 自由度

public:
  StudentTDistribution(double df = 10.0) : m_df(df) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "自由度 (df)"; }
  QString getParam2Name() const override { return ""; }
  QString getName() const override { return "学生t分布"; }
  DistType getType() const override { return DistType::StudentT; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    Q_UNUSED(p2);
    m_df = std::max(1.0, p1); // 自由度必须至少为1
  }
};

class ChiSquareDistribution : public BaseDistribution {
private:
  double m_df; // 自由度

public:
  ChiSquareDistribution(double df = 5.0) : m_df(df) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "自由度 (df)"; }
  QString getParam2Name() const override { return ""; }
  QString getName() const override { return "卡方分布"; }
  DistType getType() const override { return DistType::ChiSquare; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    Q_UNUSED(p2);
    m_df = std::max(1.0, p1); // 自由度必须至少为1
  }
};

class BetaDistribution : public BaseDistribution {
private:
  double m_alpha;
  double m_beta;

public:
  BetaDistribution(double alpha = 1.0, double beta = 1.0)
      : m_alpha(alpha), m_beta(beta) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "形状参数 α"; }
  QString getParam2Name() const override { return "形状参数 β"; }
  QString getName() const override { return "Beta分布"; }
  DistType getType() const override { return DistType::Beta; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    m_alpha = std::max(0.01, p1); // α 必须大于 0
    m_beta = std::max(0.01, p2);  // β 必须大于 0
  }
};

class GeometricDistribution : public BaseDistribution {
private:
  double m_p;

public:
  GeometricDistribution(double p = 0.5) : m_p(p) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "成功概率 (p)"; }
  QString getParam2Name() const override { return ""; }
  QString getName() const override { return "几何分布"; }
  DistType getType() const override { return DistType::Geometric; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Discrete;
  }
  void setParameters(double p1, double p2) override {
    Q_UNUSED(p2);
    m_p = std::clamp(p1, 0.01, 0.99);
  }
};

class HypergeometricDistribution : public BaseDistribution {
private:
  int m_N;
  int m_K;
  int m_n;

public:
  HypergeometricDistribution(int N = 20, int K = 10, int n = 5)
      : m_N(N), m_K(K), m_n(n) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "总体容量 (N)"; }
  QString getParam2Name() const override { return "成功数目 (K)"; }
  QString getName() const override { return "超几何分布"; }
  DistType getType() const override { return DistType::Hypergeometric; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Discrete;
  }
  void setParameters(double p1, double p2) override {
    m_N = std::max(1, (int)p1);
    m_K = std::clamp((int)p2, 0, m_N);
    m_n = std::max(1, m_N / 2);
  }
  int getN() const { return m_N; }
  int getK() const { return m_K; }
  int getn() const { return m_n; }
};

struct DistFactory {
  static BaseDistribution *create(DistType type) {
    switch (type) {
    case DistType::Normal:
      return new NormalDistribution(0.0, 1.0);
    case DistType::Binomial:
      return new BinomialDistribution(10, 0.5);
    case DistType::Poission:
      return new PoissonDistribution(5.0);
    case DistType::Uniform:
      return new UniformDistribution(0.0, 1.0);
    case DistType::Exponential:
      return new ExponentialDistribution(1.0);
    case DistType::StudentT:
      return new StudentTDistribution(10.0);
    case DistType::ChiSquare:
      return new ChiSquareDistribution(5.0);
    case DistType::Beta:
      return new BetaDistribution(1.0, 1.0);
    case DistType::Geometric:
      return new GeometricDistribution(0.5);
    case DistType::Hypergeometric:
      return new HypergeometricDistribution(20, 10, 5);
    default:
      return new NormalDistribution(0.0, 1.0);
    }
  }
};

#endif // DISTRIBUTIONMODEL_H
