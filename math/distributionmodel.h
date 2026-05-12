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
           "差，决定分布的离散程度。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>μ（均值）</"
           "b>"
           "：改变μ会使整条分布曲线左右平移。μ增大，曲线向右移动；μ减小，曲线向"
           "左移动。<br/>"
           "• "
           "<b>σ（标准差）</"
           "b>"
           "：σ控制分布的离散程度。σ越大，曲线越扁平且峰值越低，数据越分散；σ越"
           "小，曲线越尖锐且峰值越高，数据越集中。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：质量控制中的零件尺寸分析</b><br/><br/>"
           "某汽车零件制造商生产发动机活塞销，规定直径标准为50."
           "00mm。根据历史数据，"
           "实际加工误差服从正态分布 N(50.00, 0.05²)。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "工程师首先收集1000个样本，测量其直径并计算均值和标准差。假设得到μ="
           "50.00mm，σ=0.05mm。"
           "根据正态分布特性，约68%的零件落在μ±1σ范围内（49.95~50.05mm），约95%"
           "落在μ±2σ范围内，"
           "约99.7%落在μ±3σ范围内。<br/><br/>"
           "质检部门设定规格上限50.10mm和下限49.90mm。通过计算：<br/>"
           "P(49.90&lt;X&lt;50.10) = P(X&lt;50.10) - P(X&lt;49.90) ≈ 0.9772 - "
           "0.0228 ≈ 0.9544<br/><br/>"
           "这意味着约95.44%的零件合格。如果σ增大到0.08mm，合格率将下降到约85%"
           "，"
           "此时需要调整工艺或放宽标准。通过正态分布，质量部门可以量化预测不合"
           "格率，优化生产流程。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\frac{1}{\\sigma\\sqrt{2\\pi}} "
           "e^{-\\frac{(x-\\mu)^2}{2\\sigma^2}} \\)";
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
           "每次试验成功的概率。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• <b>n（试验次数）</b>：增加n会使分布的取值范围扩大（从0到n），"
           "同时分布形状趋向正态分布（当n较大时）。<br/>"
           "• <b>p（成功概率）</b>：p决定分布的偏斜方向。p=0.5时分布对称；"
           "p&lt;0.5时分布右偏（峰值偏左）；p&gt;0.5时分布左偏（峰值偏右）。"
           "p接近0或1时，分布高度集中在极端值附近。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：医疗试验中的药物有效性分析</b><br/><br/>"
           "某制药公司研发新型感冒药，需要进行临床试验。给200名患者服用该药物，"
           "已知该药物对类似病症的有效率约为70%。用二项分布 B(n=200, p=0.7) "
           "建模。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "试验负责人想知道：有多少患者服药后症状明显改善？<br/><br/>"
           "1. <b>预测最可能的好转人数</b>：期望值 E(X) = np = 200 × 0.7 = "
           "140人。<br/><br/>"
           "2. <b>计算恰好130人好转的概率</b>：<br/>"
           "P(X=130) = C(200,130) × 0.7¹³⁰ × 0.3⁷⁰ ≈ "
           "0.028（通过二项公式计算）<br/><br/>"
           "3. <b>计算至少150人好转的概率</b>：<br/>"
           "P(X≥150) = Σ P(X=k) for k=150 to 200 ≈ 0.003<br/><br/>"
           "如果实际试验中只有120人好转（显著低于期望140人），则可能说明：药物"
           "效果不如预期，"
           "或存在个体差异，需要进一步研究。二项分布帮助制药公司量化评估药物效"
           "果，做出科学决策。";
  }
  QString getFunctionExpression() const override {
    return "\\( P(X=k) = \\binom{n}{k} p^k (1-p)^{n-k} \\)";
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
           "空间内事件发生的平均次数。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>λ（率参数）</"
           "b>：λ是事件的平均发生率。λ越大，分布的峰值向右移动，"
           "分布越来越近似正态分布。λ越小，分布越向右偏斜。泊松分布的均值和方差"
           "都等于λ。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：呼叫中心的话务量预测</b><br/><br/>"
           "某银行客服中心每天接到客户来电的平均数为60通（λ="
           "60）。呼叫中心经理需要："
           "1）评估客服人员配置；2）预测某时段接到特定数量电话的概率。<br/><br/"
           ">"
           "<b>分布如何使用：</b><br/>"
           "使用泊松分布 P(X=k) = (e^(-λ) × λ^k) / k! 建模：<br/><br/>"
           "1. <b>预测某小时的话务量</b>：每小时平均来电 "
           "λ=60/8=7.5通。<br/><br/>"
           "2. <b>计算某小时恰好接到5通电话的概率</b>：<br/>"
           "P(X=5) = (e^(-7.5) × 7.5^5) / 5! ≈ 0.109<br/><br/>"
           "3. <b>计算某小时接到至少10通电话的概率</b>：<br/>"
           "P(X≥10) = 1 - P(X≤9) ≈ 1 - 0.862 = 0.138<br/><br/>"
           "根据计算，如果安排3名客服（每名处理能力约4通/"
           "小时），则某小时处理不过来的概率约13.8%。"
           "如果λ突然上升到10通/"
           "小时（如促销期间），则P(X≥10)会显著增加到约42%，"
           "此时需要增加客服人员。泊松分布帮助呼叫中心科学配置人力，平衡成本与"
           "服务水平。";
  }
  QString getFunctionExpression() const override {
    return "\\( P(X=k) = \\frac{e^{-\\lambda} \\lambda^k}{k!} \\)";
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
    return "均匀分布是指在区间[a, b]内所有值出现的概率相等的连续分布。"
           "a是区间下限，b是区间上限。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>a（下限）</"
           "b>：增大a会使整条概率密度曲线向右平移，分布的起始点改变。<br/>"
           "• "
           "<b>b（上限）</"
           "b>"
           "：增大b会使曲线范围扩大，同时概率密度高度降低（因为曲线下方面积必须"
           "为1）。<br/>"
           "• "
           "<b>区间宽度(b-a)</"
           "b>：区间越宽，概率密度越低；区间越窄，概率密度越高。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：随机数生成器与蒙特卡洛模拟</b><br/><br/>"
           "某保险公司使用蒙特卡洛模拟方法评估新型保险产品的风险。需要生成大量"
           "随机数来模拟："
           "1）客户的索赔金额（假设在1000-"
           "50000元之间均匀分布）；2）两次索赔的时间间隔。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用均匀分布 U(a=1000, b=50000) 模拟索赔金额：<br/><br/>"
           "1. <b>生成随机索赔金额</b>：计算机生成0-1之间的随机数r，通过变换 X "
           "= a + (b-a)×r 得到索赔金额。"
           "例如r=0.3时，X = 1000 + 49000×0.3 = 15700元。<br/><br/>"
           "2. <b>计算平均索赔金额</b>：E(X) = (a+b)/2 = (1000+50000)/2 = "
           "25500元。<br/><br/>"
           "3. <b>计算索赔金额超过30000元的概率</b>：<br/>"
           "P(X&gt;30000) = (b-30000)/(b-a) = 20000/49000 ≈ 40.8%<br/><br/>"
           "运行10万次模拟后，保险公司可以估算：总索赔金额的分布、理赔高峰时段"
           "、需要准备的储备金等。"
           "均匀分布因其简单性，成为蒙特卡洛模拟中最基础的工具之一。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\frac{1}{b-a}, \\quad a \\leq x \\leq b \\)";
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
           "生"
           "的平均次数。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>λ（率参数）</"
           "b>：λ越大，事件发生的平均间隔时间越短，分布峰值越靠左。"
           "λ越小，事件间隔越长，分布越平坦。指数分布的期望为1/λ。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：电子元件寿命分析与可靠性工程</b><br/><br/>"
           "某显示器制造商知道其产品的背光灯寿命服从指数分布，平均使用寿命为250"
           "00小时（λ=1/25000）。"
           "质量部门需要：1）预测元件的可靠寿命；2）制定保修策略。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用指数分布 f(x) = λe^(-λx)，其中λ=1/25000≈0.00004：<br/><br/>"
           "1. <b>计算元件在10000小时内失效的概率</b>：<br/>"
           "P(X≤10000) = 1 - e^(-λx) = 1 - e^(-0.00004×10000) = 1 - e^(-0.4) ≈ "
           "1 - 0.6703 = 0.3297<br/><br/>"
           "2. <b>计算元件使用5年后（43800小时）仍然正常工作的概率</b>：<br/>"
           "P(X&gt;43800) = e^(-λ×43800) = e^(-1.752) ≈ "
           "0.1735（仅约17%的产品能撑过5年）<br/><br/>"
           "3. "
           "<b>确定保修期</b>：若想确保至少95%的产品在保修期内不出故障，<br/>"
           "则需满足 e^(-λx) ≥ 0.95，解得 x ≤ -ln(0.95)/λ ≈ "
           "1283小时。<br/><br/>"
           "指数分布还具有无记忆性：无论元件已经使用了多久，它在下一个时段失效"
           "的概率都相同。"
           "这使得指数分布在可靠性工程和排队论中广泛应用。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\lambda e^{-\\lambda x}, \\quad x \\geq 0 \\)";
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
    return "学生t分布是小样本情况下用于估计正态分布均值的概率分布。"
           "df是自由度，当df很大时近似于正态分布。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>df（自由度）</"
           "b>：df控制分布的尾部厚度。df越小，分布的尾部越厚，峰值越低；"
           "df越大，分布越接近正态分布。通常df≥30时可近似为正态分布。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：小样本医学试验中的药物有效性检验</b><br/><br/>"
           "某制药公司研发新型降压药，由于药物稀缺，只能对12名患者进行试验。"
           "服药前收缩压均值为165mmHg，服药后均值为158mmHg，样本标准差为8mmHg。"
           "需要判断此药物是否真的有效。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "由于样本量小（n=12），总体标准差未知，使用t分布进行假设检验：<br/"
           "><br/>"
           "1. <b>计算t统计量</b>：<br/>"
           "t = (X̄ - μ₀) / (s/√n) = (158 - 165) / (8/√12) = -7 / 2.309 = "
           "-3.032<br/><br/>"
           "2. "
           "<b>确定临界值</"
           "b>：在显著性水平α=0.05下，df=11的临界值为±2.201。<br/><br/>"
           "3. <b>做出判断</b>：|t|=3.032 > 2.201，落入拒绝域，因此拒绝原假设，"
           "认为药物确实有降压效果。<br/><br/>"
           "如果使用正态分布而非t分布（如df足够大时），会得到相同结论，但小样本"
           "情况下，"
           "t分布考虑了样本量不足带来的不确定性，提供了更准确的推断。t分布在置"
           "信区间估计、"
           "回归分析中参数的显著性检验等领域都有广泛应用。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = "
           "\\frac{\\Gamma\\left(\\frac{df+1}{2}\\right)}{\\sqrt{df\\pi} "
           "\\Gamma\\left(\\frac{df}{2}\\right)} "
           "\\left(1+\\frac{x^2}{df}\\right)^{-\\frac{df+1}{2}} \\)";
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
    return "卡方分布是由df个独立标准正态变量的平方和构成的分布。"
           "df是自由度。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• <b>df（自由度）</b>：df控制分布的形状。df越小，分布越向右偏斜；"
           "随着df增大，分布逐渐趋向正态分布。卡方分布的均值等于df，方差等于2df"
           "。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：遗传学中的基因型独立性检验</b><br/><br/>"
           "遗传学家孟德尔研究豌豆遗传规律，统计了黄色圆粒豌豆与绿色皱粒豌豆的"
           "杂交结果："
           "共得到556株后代，其中黄色圆粒315株、黄色皱粒101株、绿色圆粒108株、"
           "绿色皱粒32株。"
           "根据遗传学理论，这四类比例应为9:3:3:"
           "1。需要检验观察结果是否符合理论预期。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用卡方拟合优度检验：<br/><br/>"
           "1. <b>计算期望频数</b>：总株数556，按9:3:3:1比例分配：<br/>"
           "黄色圆粒：556 × 9/16 = 312.75；黄色皱粒：556 × 3/16 = 104.25；<br/>"
           "绿色圆粒：556 × 3/16 = 104.25；绿色皱粒：556 × 1/16 = "
           "34.75<br/><br/>"
           "2. <b>计算卡方统计量</b>：<br/>"
           "χ² = Σ(观察频数-期望频数)²/期望频数<br/>"
           "= (315-312.75)²/312.75 + (101-104.25)²/104.25 + "
           "(108-104.25)²/104.25 + (32-34.75)²/34.75<br/>"
           "≈ 0.016 + 0.101 + 0.135 + 0.218 = 0.47<br/><br/>"
           "3. "
           "<b>做出判断</b>：自由度=类别数-1=4-1=3，在α=0.05下临界值为7.815。"
           "χ²=0.47 &lt; 7.815，因此接受原假设，观察结果与理论比例无显著差异。"
           "卡方分布在社会学、医学、市场研究等领域都有广泛应用。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\frac{x^{\\frac{df}{2}-1} "
           "e^{-\\frac{x}{2}}}{2^{\\frac{df}{2}} "
           "\\Gamma\\left(\\frac{df}{2}\\right)} \\)";
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
    return "Beta分布是定义在[0,1]区间上的连续分布。α和β是形状参数，"
           "控制分布的形态。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>α（形状参数）</"
           "b>：α增大，分布峰值向右移动；当α&lt;1且β≥1时，分布下凹；当α&gt;"
           "1时，分布开始变得对称。<br/>"
           "• "
           "<b>β（形状参数）</"
           "b>：β增大，分布峰值向左移动。当α=β=1时退化为均匀分布；当α="
           "β且大于1时，分布对称且峰值越高。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：贝叶斯统计中的产品合格率估计</b><br/><br/>"
           "某芯片制造商想要估计新一代芯片的合格率。由于生产规模限制，只能测试2"
           "0片，"
           "其中有18片合格。先验信息认为合格率可能在0.7-0."
           "95之间。使用Beta分布进行贝叶斯估计。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "将先验分布设为Beta(α=2, "
           "β=1)，这是一个偏向高合格率的先验：<br/><br/>"
           "1. "
           "<b>确定先验</"
           "b>：Beta(2,1)的密度在x=1处达到峰值，表示高合格率的可能性更大。<br/"
           "><br/>"
           "2. "
           "<b>更新后验</"
           "b>：根据贝叶斯定理，试验结果（18成功，2失败）更新后验分布为：<br/>"
           "后验 ∝ 先验 × 似然 = Beta(2,1) × Binomial(18|20,p)<br/>"
           "后验分布为 Beta(α+18, β+2) = Beta(20, 3)<br/><br/>"
           "3. <b>计算后验估计</b>：后验均值 = 20/(20+3) ≈ 0.87，后验标准差 ≈ "
           "0.07。<br/><br/>"
           "4. "
           "<b>做出预测</"
           "b>：有95%的概率合格率高于0."
           "65。如果增加测试样本量，后验分布会更窄，估计更精确。"
           "Beta分布在贝叶斯统计中是二项分布的共轭先验，被广泛用于概率参数的估"
           "计。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\frac{x^{\\alpha-1} "
           "(1-x)^{\\beta-1}}{B(\\alpha,\\beta)} \\)";
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
    return "几何分布描述首次成功所需的试验次数。p是每次试验成功的概率。<br/"
           "><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>p（成功概率）</"
           "b>：p越大，首次成功越可能发生在早期，分布峰值向左移动；"
           "p越小，首次成功越可能需要很多次试验，分布越向右延伸。p接近1时，分布"
           "极度集中在k=1附近。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：游戏设计中的抽卡系统概率分析</b><br/><br/>"
           "某手游的抽卡系统设定：每次抽卡获得稀有角色的概率为5%（p=0.05）。"
           "玩家想知道：需要抽多少次才能首次获得稀有角色？开发者需要评估抽卡系"
           "统的平均收益。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用几何分布 P(X=k) = (1-p)^(k-1) × p建模：<br/><br/>"
           "1. <b>计算平均抽卡次数</b>：E(X) = 1/p = 1/0.05 = "
           "20次。平均而言，玩家需要抽20次才能首次获得稀有角色。<br/><br/>"
           "2. <b>计算在10次内抽到的概率</b>：<br/>"
           "P(X≤10) = 1 - (1-p)^10 = 1 - 0.95^10 ≈ 1 - 0.5987 = "
           "0.4013<br/><br/>"
           "3. <b>计算需要超过50次的概率</b>：<br/>"
           "P(X&gt;50) = (1-p)^50 = 0.95^50 ≈ "
           "0.077（仅约7.7%的玩家需要抽50次以上）<br/><br/>"
           "4. "
           "<b>保底机制设计</"
           "b>：如果开发者设置\"50抽保底\"，则约92.3%"
           "的玩家会在50抽内获得稀有角色，"
           "确保大多数玩家有良好体验。几何分布帮助游戏设计师平衡付费玩家与非付"
           "费玩家的利益，优化游戏经济系统。";
  }
  QString getFunctionExpression() const override {
    return "\\( P(X=k) = (1-p)^{k-1} p \\)";
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
    return "超几何分布描述从有限总体中无放回抽样时成功次数的概率分布。"
           "N是总体容量，K是总体中成功元素数，n是样本容量。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>N（总体容量）</"
           "b>"
           "：N越大，超几何分布越接近二项分布（因为无放回抽样的相对影响减小）。"
           "<br/>"
           "• "
           "<b>K（成功数目）</b>：K/"
           "N决定了成功比例。K越大，成功次数的期望值越高。<br/>"
           "• "
           "<b>n（样本容量）</b>：n越大，分布的方差越大，但期望始终为n×(K/N)。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：质量检测中的无放回抽样方案</b><br/><br/>"
           "某零件供应商送货1000件，其中已知有50件可能是次品（K="
           "50）。采购方随机抽取100件进行检测（n=100）。"
           "需要根据抽检结果决定是否接受这批货。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用超几何分布 P(X=k) = C(K,k)×C(N-K,n-k)/C(N,n) 建模：<br/><br/>"
           "1. <b>计算期望次品数</b>：E(X) = n×(K/N) = 100×(50/1000) = "
           "5件。<br/><br/>"
           "2. <b>计算恰好抽到3件次品的概率</b>：<br/>"
           "P(X=3) = C(50,3)×C(950,97)/C(1000,100) ≈ 0.181<br/><br/>"
           "3. <b>计算抽到不超过2件次品的概率</b>：<br/>"
           "P(X≤2) = P(X=0)+P(X=1)+P(X=2) ≈ 0.376<br/><br/>"
           "4. "
           "<b>制定接收准则</"
           "b>：如果抽检中次品数≤2，则接收这批货；次品数≥5，则退货。"
           "这种方案下，即使整批货真的有50件次品（5%次品率），也有约64%"
           "的概率被接收（批容判断）。"
           "超几何分布帮助质检部门在不完全检测的情况下，科学评估整批货物的质量"
           "。";
  }
  QString getFunctionExpression() const override {
    return "\\( P(X=k) = \\frac{\\binom{K}{k} "
           "\\binom{N-K}{n-k}}{\\binom{N}{n}} \\)";
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
    return "F分布描述两个独立卡方分布除以各自自由度后比值的概率分布。"
           "df₁是分子自由度，df₂是分母自由度。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>df₁（分子自由度）</"
           "b>：df₁越大，F分布的峰值越靠左，分布越集中。<br/>"
           "• "
           "<b>df₂（分母自由度）</"
           "b>：df₂越大，F分布的尾部越薄，分布越近似正态分布。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：农业试验中的小麦品种产量比较</b><br/><br/>"
           "农业科学家比较两种小麦品种（品种A和品种B）的产量。各在5块试验田中种"
           "植，"
           "测得品种A产量方差为s₁²=120，品种B产量方差为s₂²="
           "80。需要判断两种品种的产量稳定性是否存在显著差异。<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用F分布进行方差齐性检验：<br/><br/>"
           "1. <b>计算F统计量</b>：<br/>"
           "F = s₁²/s₂² = 120/80 = 1.5<br/><br/>"
           "2. "
           "<b>确定自由度</"
           "b>：df₁=5-1=4（品种A自由度），df₂=5-1=4（品种B自由度）。<br/><br/>"
           "3. <b>查表或计算p值</b>：在α=0.05下，F分布临界值为6.39。由于F=1.5 "
           "&lt; 6.39，不能拒绝原假设，"
           "即两种品种的产量稳定性无显著差异。<br/><br/>"
           "F分布在方差分析（ANOVA）中用于比较多个组的均值差异，在回归分析中用"
           "于检验整体回归方程的显著性，"
           "是统计推断中非常重要的工具。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = "
           "\\frac{\\Gamma\\left(\\frac{df_1+df_2}{2}\\right)}{\\Gamma\\left("
           "\\frac{df_1}{2}\\right) \\Gamma\\left(\\frac{df_2}{2}\\right)} "
           "\\left(\\frac{df_1}{df_2}\\right)^{\\frac{df_1}{2}} "
           "\\frac{x^{\\frac{df_1}{2}-1}}{\\left(1 + "
           "\\frac{df_1}{df_2}x\\right)^{\\frac{df_1+df_2}{2}}} \\)";
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
    return "伽马分布是连续概率分布，用于描述独立事件发生的时间间隔的总和。"
           "α是形状参数，β是率参数。当α=1时退化为指数分布。<br/><br/>"
           "<b>参数影响：</b><br/>"
           "• "
           "<b>α（形状参数）</"
           "b>：α控制分布的形状。α=1时退化为指数分布；α&lt;1时分布下凹；α&gt;"
           "1时分布单峰且越增大越对称。<br/>"
           "• "
           "<b>β（率参数）</"
           "b>"
           "：β影响分布的尺度。β越大，分布峰值越靠左，范围越窄；β越小，分布越平"
           "坦。伽马分布的均值=α/β。";
  }
  QString getUsageScenario() const override {
    return "<b>典型案例：排队论中的多窗口服务系统分析</b><br/><br/>"
           "某银行有3个服务窗口（α="
           "3），每个窗口顾客服务时间服从指数分布，平均服务时间为10分钟（β=0."
           "1）。"
           "经理需要预测：一位顾客完成所有服务（3个窗口都办完业务）所需的平均时"
           "间是多少？同时等待概率是多少？<br/><br/>"
           "<b>分布如何使用：</b><br/>"
           "使用伽马分布 Γ(α=3, β=0.1) 建模总服务时间：<br/><br/>"
           "1. <b>计算平均总服务时间</b>：E(X) = α/β = 3/0.1 = "
           "30分钟。<br/><br/>"
           "2. <b>计算总服务时间超过1小时的概率</b>：<br/>"
           "P(X&gt;60) = 1 - P(X≤60) ≈ "
           "0.0025（仅约0.25%的顾客需要超过1小时）<br/><br/>"
           "3. "
           "<b>评估服务质量</b>：如果经理希望95%的顾客在45分钟内完成所有服务，"
           "需要计算 P(X≤45) "
           "是否达到0.95。若不足，则需要增加窗口或优化服务流程。<br/><br/>"
           "伽马分布还用于保险精算（巨灾损失建模）、可靠度工程（复杂系统失效时"
           "间）、贝叶斯统计（指数分布的共轭先验）等领域。";
  }
  QString getFunctionExpression() const override {
    return "\\( f(x) = \\frac{\\beta^\\alpha}{\\Gamma(\\alpha)} x^{\\alpha-1} "
           "e^{-\\beta x}, \\quad x \\geq 0 \\)";
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
    case DistType::Uniform:
      relations.append(
          {DistType::Uniform,
           "通过逆变换采样(Inverse Transform "
           "Sampling)，均匀分布可以生成任何分布。这是计算机模拟的核心。"});
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
