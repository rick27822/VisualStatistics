#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include <QList>
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
  Hypergeometric,
  FDistribution,
  Gamma
};
enum class DistributionCategory { Continuous, Discrete };

class BaseDistribution {
public:
  virtual ~BaseDistribution() {}
  virtual DistType getType() const = 0;                 // 获取分布类型
  virtual DistributionCategory getCategory() const = 0; // 获取分布类别
  virtual double calculate(double x) const = 0;         // 计算概率密度
  virtual double calculateCDF(double x) const = 0;
  virtual QPair<double, double> getSuggestedRange() const = 0; // 获取建议的范围
  virtual QString getName() const = 0;                         // 获取分布名称

  virtual QString getParam1Name() const = 0;
  virtual QString getParam2Name() const = 0;
  virtual void setParameters(double p1, double p2) = 0; // 更新参数

  virtual int getParamCount() const = 0;
  virtual QList<QString> getParamNames() const = 0;
  virtual QList<double> getParamDefaults() const = 0;
  virtual QList<QPair<double, double>> getParamRanges() const = 0;

  virtual QString getDescription() const = 0;
  virtual QString getUsageScenario() const = 0;
  virtual QString getFunctionExpression() const = 0;
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
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"期望 (μ)", "标准差 (σ)"};
  }
  QList<double> getParamDefaults() const override { return {0.0, 1.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{-100.0, 100.0}, {0.1, 20.0}};
  }
  QString getDescription() const override {
    return "正态分布是最常见的连续概率分布。μ是均值，决定分布中心位置；σ是标准"
           "差，决定分布的离散程度。";
  }
  QString getUsageScenario() const override {
    return "用于描述自然界和社会科学中许多现象，如身高、体重、考试成绩等。中心"
           "极限定理表明，大量独立随机变量的和近似服从正态分布。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = (1/(σ√(2π))) * exp(-(x-μ)²/(2σ²))";
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
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"实验次数 (n)", "概率 (p)"};
  }
  QList<double> getParamDefaults() const override { return {10.0, 0.5}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{1.0, 100.0}, {0.0, 1.0}};
  }
  QString getDescription() const override {
    return "二项分布描述n次独立伯努利试验中成功次数的概率分布。n是试验次数，p是"
           "每次试验成功的概率。";
  }
  QString getUsageScenario() const override {
    return "用于计算在n次独立试验中恰好成功k次的概率，如抛硬币、产品抽检、问卷"
           "调查等场景。";
  }
  QString getFunctionExpression() const override {
    return "P(X=k) = C(n,k) * p^k * (1-p)^(n-k)";
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
  int getParamCount() const override { return 1; }
  QList<QString> getParamNames() const override { return {"均值 (λ)"}; }
  QList<double> getParamDefaults() const override { return {5.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{0.01, 50.0}};
  }
  QString getDescription() const override {
    return "泊松分布描述在固定时间或空间内发生指定次数事件的概率。λ是单位时间/"
           "空间内事件发生的平均次数。";
  }
  QString getUsageScenario() const override {
    return "用于描述稀有事件的发生次数，如电话交换机接到的呼叫次数、放射性衰变"
           "次数、网站访问量等。";
  }
  QString getFunctionExpression() const override {
    return "P(X=k) = (e^(-λ) * λ^k) / k!";
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
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"下限 (a)", "上限 (b)"};
  }
  QList<double> getParamDefaults() const override { return {0.0, 1.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{-100.0, 100.0}, {-100.0, 100.0}};
  }
  QString getDescription() const override {
    return "均匀分布是指在区间[a, "
           "b]内所有值出现的概率相等的连续分布。a是区间下限，b是区间上限。";
  }
  QString getUsageScenario() const override {
    return "用于模拟等概率事件，如随机数生成、掷骰子、抽奖等场景。也用于蒙特卡"
           "洛模拟。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = 1/(b-a), a ≤ x ≤ b";
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
  int getParamCount() const override { return 1; }
  QList<QString> getParamNames() const override { return {"率参数 (λ)"}; }
  QList<double> getParamDefaults() const override { return {1.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{0.01, 20.0}};
  }
  QString getDescription() const override {
    return "指数分布描述独立事件发生的时间间隔。λ是率参数，表示单位时间内事件发"
           "生的平均次数。";
  }
  QString getUsageScenario() const override {
    return "用于描述等待时间，如电子元件寿命、顾客到达间隔时间、设备故障间隔等"
           "。具有无记忆性。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = λ * e^(-λx), x ≥ 0";
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
  int getParamCount() const override { return 1; }
  QList<QString> getParamNames() const override { return {"自由度 (df)"}; }
  QList<double> getParamDefaults() const override { return {10.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{1.0, 100.0}};
  }
  QString getDescription() const override {
    return "学生t分布是小样本情况下用于估计正态分布均值的概率分布。df是自由度，"
           "当df很大时近似于正态分布。";
  }
  QString getUsageScenario() const override {
    return "用于小样本统计推断，如总体标准差未知时的均值检验、置信区间估计等。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = Γ((df+1)/2) / (√(dfπ) * Γ(df/2)) * (1+x²/df)^(-(df+1)/2)";
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
  int getParamCount() const override { return 1; }
  QList<QString> getParamNames() const override { return {"自由度 (df)"}; }
  QList<double> getParamDefaults() const override { return {5.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{1.0, 100.0}};
  }
  QString getDescription() const override {
    return "卡方分布是由df个独立标准正态变量的平方和构成的分布。df是自由度。";
  }
  QString getUsageScenario() const override {
    return "用于拟合优度检验、独立性检验、方差估计等统计推断场景。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = x^(df/2-1) * e^(-x/2) / (2^(df/2) * Γ(df/2))";
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
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"形状参数 α", "形状参数 β"};
  }
  QList<double> getParamDefaults() const override { return {1.0, 1.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{0.01, 20.0}, {0.01, 20.0}};
  }
  QString getDescription() const override {
    return "Beta分布是定义在[0,1]"
           "区间上的连续分布。α和β是形状参数，控制分布的形态。";
  }
  QString getUsageScenario() const override {
    return "用于表示概率的概率分布，如贝叶斯统计中的先验分布、时间比例数据建模"
           "等。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = x^(α-1) * (1-x)^(β-1) / B(α,β)";
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
  int getParamCount() const override { return 1; }
  QList<QString> getParamNames() const override { return {"成功概率 (p)"}; }
  QList<double> getParamDefaults() const override { return {0.5}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{0.01, 0.99}};
  }
  QString getDescription() const override {
    return "几何分布描述首次成功所需的试验次数。p是每次试验成功的概率。";
  }
  QString getUsageScenario() const override {
    return "用于计算首次成功前的失败次数，如产品首次合格前的次品数、首次中奖前"
           "的抽奖次数等。";
  }
  QString getFunctionExpression() const override {
    return "P(X=k) = (1-p)^(k-1) * p";
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
  void setN(int N) { m_N = std::max(1, N); }
  void setK(int K) { m_K = std::clamp(K, 0, m_N); }
  void setn(int n) { m_n = std::max(1, std::min(n, m_N)); }
  int getParamCount() const override { return 3; }
  QList<QString> getParamNames() const override {
    return {"总体容量 (N)", "成功数目 (K)", "样本容量 (n)"};
  }
  QList<double> getParamDefaults() const override { return {20.0, 10.0, 5.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{5.0, 100.0}, {1.0, 100.0}, {1.0, 50.0}};
  }
  QString getDescription() const override {
    return "超几何分布描述从有限总体中无放回抽样时成功次数的概率分布。N是总体容"
           "量，K是总体中成功元素数，n是样本容量。";
  }
  QString getUsageScenario() const override {
    return "用于无放回抽样场景，如从一批产品中抽检、从有限总体中抽样调查等。";
  }
  QString getFunctionExpression() const override {
    return "P(X=k) = C(K,k) * C(N-K,n-k) / C(N,n)";
  }
};

class FDistribution : public BaseDistribution {
private:
  double m_df1; // 分子自由度
  double m_df2; // 分母自由度

public:
  FDistribution(double df1 = 10.0, double df2 = 10.0)
      : m_df1(df1), m_df2(df2) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "分子自由度 df₁"; }
  QString getParam2Name() const override { return "分母自由度 df₂"; }
  QString getName() const override { return "F分布"; }
  DistType getType() const override { return DistType::FDistribution; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    m_df1 = std::max(1.0, p1); // 自由度至少为1
    m_df2 = std::max(1.0, p2);
  }
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"分子自由度 df₁", "分母自由度 df₂"};
  }
  QList<double> getParamDefaults() const override { return {10.0, 10.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{1.0, 100.0}, {1.0, 100.0}};
  }
  QString getDescription() const override {
    return "F分布描述两个独立卡方分布除以各自自由度后比值的概率分布。df₁是分子"
           "自由度，df₂是分母自由度。";
  }
  QString getUsageScenario() const override {
    return "用于方差分析、回归方程显著性检验、两个总体方差比的检验等统计推断场"
           "景。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = Γ((df₁+df₂)/2) * (df₁/df₂)^(df₁/2) * x^(df₁/2-1) / ( "
           "Γ(df₁/2) * Γ(df₂/2) * (1 + (df₁/df₂)x)^((df₁+df₂)/2))";
  }
};

class GammaDistribution : public BaseDistribution {
private:
  double m_alpha; // 形状参数
  double m_beta;  // 率参数

public:
  GammaDistribution(double alpha = 1.0, double beta = 1.0)
      : m_alpha(alpha), m_beta(beta) {}
  double calculate(double x) const override;
  double calculateCDF(double x) const override;
  QPair<double, double> getSuggestedRange() const override;
  QString getParam1Name() const override { return "形状参数 α"; }
  QString getParam2Name() const override { return "率参数 β"; }
  QString getName() const override { return "伽马分布"; }
  DistType getType() const override { return DistType::Gamma; }
  DistributionCategory getCategory() const override {
    return DistributionCategory::Continuous;
  }
  void setParameters(double p1, double p2) override {
    m_alpha = std::max(0.01, p1); // α 必须大于 0
    m_beta = std::max(0.01, p2);  // β 必须大于 0
  }
  int getParamCount() const override { return 2; }
  QList<QString> getParamNames() const override {
    return {"形状参数 α", "率参数 β"};
  }
  QList<double> getParamDefaults() const override { return {1.0, 1.0}; }
  QList<QPair<double, double>> getParamRanges() const override {
    return {{0.01, 20.0}, {0.01, 20.0}};
  }
  QString getDescription() const override {
    return "伽马分布是连续概率分布，用于描述独立事件发生的时间间隔的总和。α是形"
           "状"
           "参数，β是率参数。当α=1时退化为指数分布。";
  }
  QString getUsageScenario() const override {
    return "用于描述等待多个事件发生的时间，如排队论中的服务时间、可靠性分析中"
           "的"
           "失效时间等。也常用于贝叶斯统计中的先验分布。";
  }
  QString getFunctionExpression() const override {
    return "f(x) = (β^α / Γ(α)) * x^(α-1) * e^(-βx), x ≥ 0";
  }
};

struct RelatedDistribution {
  DistType relatedType;
  QString relationDescription;
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
    case DistType::FDistribution:
      return new FDistribution(10.0, 10.0);
    case DistType::Gamma:
      return new GammaDistribution(1.0, 1.0);
    default:
      return new NormalDistribution(0.0, 1.0);
    }
  }

  static QList<RelatedDistribution> getRelatedDistributions(DistType type) {
    QList<RelatedDistribution> relations;
    switch (type) {
    case DistType::Normal:
      relations.append(
          {DistType::Binomial,
           "二项分布在n很大时（De Moivre-Laplace定理，近似为正态分布"});
      relations.append(
          {DistType::ChiSquare, "卡方分布由独立标准正态变量平方和构成"});
      relations.append({DistType::StudentT, "t分布在自由度很大时近似正态分布"});
      break;
    case DistType::Binomial:
      relations.append({DistType::Normal,
                        "n很大p适中时（De Moivre-Laplace定理，近似为正态分布"});
      relations.append(
          {DistType::Poission, "n→∞，p→0且np=λ时，近似为泊松分布"});
      relations.append(
          {DistType::Hypergeometric, "总体N很大时，超几何分布近似为二项分布"});
      break;
    case DistType::Poission:
      relations.append({DistType::Binomial, "二项分布在n→∞，p→0时的极限"});
      relations.append(
          {DistType::Exponential, "指数分布描述泊松过程中事件间隔时间"});
      relations.append({DistType::Normal, "λ很大时近似为正态分布"});
      break;
    case DistType::Exponential:
      relations.append(
          {DistType::Poission, "指数分布是泊松过程中两次事件的时间间隔"});
      relations.append({DistType::Gamma, "n个独立指数变量之和服从伽马分布"});
      break;
    case DistType::StudentT:
      relations.append({DistType::Normal, "自由度很大时近似为正态分布"});
      relations.append(
          {DistType::FDistribution, "t²(df)的平方服从F(1, df)分布"});
      break;
    case DistType::ChiSquare:
      relations.append({DistType::Normal, "由独立标准正态变量平方和构成"});
      relations.append({DistType::Gamma, "当伽马(α=df/2, β=1/2)为卡方分布"});
      relations.append(
          {DistType::FDistribution, "两个独立卡方变量比服从F分布"});
      break;
    case DistType::Beta:
      relations.append(
          {DistType::Gamma, "X/(X+Y)服从Beta分布，X和Y为独立伽马变量"});
      relations.append({DistType::Uniform, "α=β=1时退化为均匀分布"});
      break;
    case DistType::Hypergeometric:
      relations.append({DistType::Binomial, "总体N很大时近似为二项分布"});
      break;
    case DistType::FDistribution:
      relations.append({DistType::ChiSquare, "由两个独立卡方变量比构成"});
      relations.append({DistType::StudentT, "t²(df) = F(1, df)"});
      break;
    case DistType::Gamma:
      relations.append({DistType::Exponential, "α=1时退化为指数分布"});
      relations.append({DistType::ChiSquare, "α=df/2, β=1/2时为卡方分布"});
      relations.append({DistType::Beta, "X/(X+Y)服从Beta分布"});
      break;
    default:
      break;
    }
    return relations;
  }
};

#endif // DISTRIBUTIONMODEL_H
