#include "distributionmodel.h"
#include <cmath>

double NormalDistribution::calculate(double x) const {
  double exponent = -0.5 * std::pow((x - m_mu) / m_sigma, 2);
  return (1.0 / (m_sigma * std::sqrt(2.0 * M_PI))) * std::exp(exponent);
}
double NormalDistribution::calculateCDF(double x) const {
  return 0.5 * (1.0 + std::erf((x - m_mu) / (m_sigma * std::sqrt(2.0))));
}
QPair<double, double> NormalDistribution::getSuggestedRange() const {
  return qMakePair(m_mu - 5 * m_sigma, m_mu + 5 * m_sigma);
}
double BinomialDistribution::calculate(double x) const {
  int k = std::round(x);
  if (k < 0 || k > n)
    return 0;
  if (p <= 0)
    return (k == 0) ? 1.0 : 0.0;
  if (p >= 1)
    return (k == n) ? 1.0 : 0.0;

  // 使用对数计算组合数公式防止溢出
  double logCnK =
      std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1);
  double logProb = logCnK + k * std::log(p) + (n - k) * std::log(1.0 - p);

  return std::exp(logProb);
}
QPair<double, double> BinomialDistribution::getSuggestedRange() const {
  return qMakePair(-1.0, double(n) + 1.0);
}
double BinomialDistribution::calculateCDF(double x) const {
  int k_max = std::floor(x);
  if (k_max < 0)
    return 0.0;
  if (k_max >= n)
    return 1.0;

  double sum = 0.0;
  for (int i = 0; i <= k_max; ++i) {
    sum += calculate(double(i));
  }
  return sum;
}

double PoissonDistribution::calculate(double x) const {
  int k = std::round(x);
  if (k < 0)
    return 0.0;
  if (m_lambda <= 0)
    return (k == 0) ? 1.0 : 0.0;

  double logFactorial = std::lgamma(k + 1);
  double logProb = -m_lambda + k * std::log(m_lambda) - logFactorial;

  return std::exp(logProb);
}

double PoissonDistribution::calculateCDF(double x) const {
  int k_max = std::floor(x);
  if (k_max < 0)
    return 0.0;

  double sum = 0.0;
  for (int i = 0; i <= k_max; ++i) {
    sum += calculate(double(i));
  }
  return sum;
}

QPair<double, double> PoissonDistribution::getSuggestedRange() const {
  double max_k = m_lambda + 4 * std::sqrt(m_lambda);
  return qMakePair(-1.0, max_k + 1.0);
}

double UniformDistribution::calculate(double x) const {
  if (x < m_a || x > m_b)
    return 0.0;
  return 1.0 / (m_b - m_a);
}

double UniformDistribution::calculateCDF(double x) const {
  if (x < m_a)
    return 0.0;
  if (x >= m_b)
    return 1.0;
  return (x - m_a) / (m_b - m_a);
}

QPair<double, double> UniformDistribution::getSuggestedRange() const {
  double range = m_b - m_a;
  return qMakePair(m_a - range * 0.5, m_b + range * 0.5);
}

double ExponentialDistribution::calculate(double x) const {
  if (x < 0)
    return 0.0;
  return m_lambda * std::exp(-m_lambda * x);
}

double ExponentialDistribution::calculateCDF(double x) const {
  if (x < 0)
    return 0.0;
  return 1.0 - std::exp(-m_lambda * x);
}

QPair<double, double> ExponentialDistribution::getSuggestedRange() const {
  return qMakePair(-1.0, 5.0 / m_lambda);
}

double StudentTDistribution::calculate(double x) const {
  // 学生t分布的概率密度函数
  double numerator = std::tgamma((m_df + 1.0) / 2.0);
  double denominator = std::sqrt(m_df * M_PI) * std::tgamma(m_df / 2.0);
  double base = 1.0 + (x * x) / m_df;
  double exponent = -(m_df + 1.0) / 2.0;
  return (numerator / denominator) * std::pow(base, exponent);
}

double StudentTDistribution::calculateCDF(double x) const {
  double df = std::max(1.0, m_df);

  if (x == 0.0)
    return 0.5;

  // 对于大自由度，使用正态近似
  if (df > 30) {
    // 正态近似
    double z = x / std::sqrt(1.0 + x * x / (2.0 * df));
    return 0.5 * (1.0 + std::erf(z / std::sqrt(2.0)));
  }

  double t = x;

  // 使用数值积分计算CDF
  // 学生t分布的PDF: f(t) = gamma((n+1)/2) / (sqrt(n*pi) * gamma(n/2)) * (1 +
  // t^2/n)^(-(n+1)/2)

  const int steps = 10000;
  double integral = 0.0;
  double sign = (t >= 0) ? 1.0 : -1.0;
  double abs_t = std::abs(t);

  // 积分从0到abs_t
  double a = 0.0;
  double b = abs_t;
  double h = (b - a) / steps;

  for (int i = 0; i < steps; ++i) {
    double x1 = a + i * h;
    double x2 = x1 + h;
    double f1 = calculate(x1);
    double f2 = calculate(x2);
    integral += (f1 + f2) * h / 2.0;
  }

  double result = 0.5 + sign * integral;

  // 确保结果在合理范围内
  return std::max(0.0, std::min(1.0, result));
}

QPair<double, double> StudentTDistribution::getSuggestedRange() const {
  // 建议的范围，根据自由度调整
  double range = 4.0 + 6.0 / (1.0 + m_df * 0.1);
  return qMakePair(-range, range);
}

double ChiSquareDistribution::calculate(double x) const {
  if (x <= 0)
    return 0.0;

  double df = m_df;
  double half_df = df / 2.0;

  double numerator = std::pow(x, half_df - 1.0) * std::exp(-x / 2.0);
  double denominator = std::pow(2.0, half_df) * std::tgamma(half_df);

  return numerator / denominator;
}

double ChiSquareDistribution::calculateCDF(double x) const {
  if (x <= 0)
    return 0.0;

  double df = std::max(1.0, m_df);
  double half_df = df / 2.0;
  double half_x = x / 2.0;

  // 正则化不完全伽马函数计算CDF
  // γ(s, x) / Γ(s)
  double result = std::tgamma(half_df);
  if (result == 0.0)
    return 1.0;

  // 使用数值积分计算不完全伽马函数
  const int steps = 10000;
  double integral = 0.0;
  double a = 0.0;
  double b = half_x;
  double h = (b - a) / steps;

  for (int i = 0; i < steps; ++i) {
    double x1 = a + i * h;
    double x2 = x1 + h;
    double f1 = (x1 > 0) ? std::pow(x1, half_df - 1.0) * std::exp(-x1) : 0.0;
    double f2 = (x2 > 0) ? std::pow(x2, half_df - 1.0) * std::exp(-x2) : 0.0;
    integral += (f1 + f2) * h / 2.0;
  }

  return std::min(1.0, std::max(0.0, integral / result));
}

QPair<double, double> ChiSquareDistribution::getSuggestedRange() const {
  double max_x = m_df + 4.0 * std::sqrt(m_df);
  return qMakePair(-1.0, max_x + 2.0);
}

double BetaDistribution::calculate(double x) const {
  if (x <= 0.0 || x >= 1.0)
    return 0.0;

  double alpha = m_alpha;
  double beta = m_beta;

  double numerator = std::pow(x, alpha - 1.0) * std::pow(1.0 - x, beta - 1.0);
  double denominator =
      std::tgamma(alpha) * std::tgamma(beta) / std::tgamma(alpha + beta);

  return numerator / denominator;
}

double BetaDistribution::calculateCDF(double x) const {
  if (x <= 0.0)
    return 0.0;
  if (x >= 1.0)
    return 1.0;

  double alpha = m_alpha;
  double beta = m_beta;

  double B = std::tgamma(alpha) * std::tgamma(beta) / std::tgamma(alpha + beta);

  const int steps = 10000;
  double integral = 0.0;
  double a = 0.0;
  double b = x;
  double h = (b - a) / steps;

  for (int i = 0; i < steps; ++i) {
    double x1 = a + i * h;
    double x2 = x1 + h;
    double f1 = (x1 > 0 && x1 < 1)
                    ? std::pow(x1, alpha - 1.0) * std::pow(1.0 - x1, beta - 1.0)
                    : 0.0;
    double f2 = (x2 > 0 && x2 < 1)
                    ? std::pow(x2, alpha - 1.0) * std::pow(1.0 - x2, beta - 1.0)
                    : 0.0;
    integral += (f1 + f2) * h / 2.0;
  }

  return std::min(1.0, std::max(0.0, integral / B));
}

QPair<double, double> BetaDistribution::getSuggestedRange() const {
  return qMakePair(-0.1, 1.1);
}

double GeometricDistribution::calculate(double x) const {
  int k = std::round(x);
  if (k < 1)
    return 0.0;
  if (m_p <= 0)
    return (k == 1) ? 1.0 : 0.0;
  if (m_p >= 1)
    return (k == 1) ? 1.0 : 0.0;

  return m_p * std::pow(1.0 - m_p, k - 1.0);
}

double GeometricDistribution::calculateCDF(double x) const {
  int k_max = std::floor(x);
  if (k_max < 1)
    return 0.0;
  return 1.0 - std::pow(1.0 - m_p, k_max);
}

QPair<double, double> GeometricDistribution::getSuggestedRange() const {
  double max_k = 5.0 / m_p;
  return qMakePair(-0.5, max_k + 1.0);
}

double HypergeometricDistribution::calculate(double x) const {
  int k = std::round(x);
  if (k < 0 || k > m_n)
    return 0.0;
  if (k > m_K)
    return 0.0;
  if (m_n - k > m_N - m_K)
    return 0.0;

  double logCombNK =
      std::lgamma(m_N + 1) - std::lgamma(m_K + 1) - std::lgamma(m_N - m_K + 1);
  double logCombKk =
      std::lgamma(m_K + 1) - std::lgamma(k + 1) - std::lgamma(m_K - k + 1);
  double logCombNKnmk = std::lgamma(m_N - m_K + 1) - std::lgamma(m_n - k + 1) -
                        std::lgamma(m_N - m_K - m_n + k + 1);
  double logCombNn =
      std::lgamma(m_N + 1) - std::lgamma(m_n + 1) - std::lgamma(m_N - m_n + 1);

  double logProb = logCombKk + logCombNKnmk - logCombNn;

  return std::exp(logProb);
}

double HypergeometricDistribution::calculateCDF(double x) const {
  int k_max = std::floor(x);
  if (k_max < 0)
    return 0.0;
  if (k_max >= m_n)
    return 1.0;

  double sum = 0.0;
  for (int i = 0; i <= k_max; ++i) {
    sum += calculate(static_cast<double>(i));
  }
  return std::min(1.0, sum);
}

QPair<double, double> HypergeometricDistribution::getSuggestedRange() const {
  return qMakePair(-1.0, static_cast<double>(m_n) + 1.0);
}

double FDistribution::calculate(double x) const {
  if (x <= 0)
    return 0.0;

  double df1 = m_df1;
  double df2 = m_df2;
  double half_df1 = df1 / 2.0;
  double half_df2 = df2 / 2.0;
  double half_sum = (df1 + df2) / 2.0;

  // F 分布的概率密度函数
  double term1 = std::tgamma(half_sum);
  double term2 = std::pow(df1 / df2, half_df1);
  double term3 = std::pow(x, half_df1 - 1.0);
  double term4 = std::tgamma(half_df1) * std::tgamma(half_df2);
  double term5 = std::pow(1.0 + (df1 / df2) * x, half_sum);

  return (term1 * term2 * term3) / (term4 * term5);
}

double FDistribution::calculateCDF(double x) const {
  if (x <= 0)
    return 0.0;

  double df1 = m_df1;
  double df2 = m_df2;

  // 使用数值积分计算 F 分布的 CDF
  const int steps = 10000;
  double integral = 0.0;
  double a = 0.0;
  double b = x;
  double h = (b - a) / steps;

  for (int i = 0; i < steps; ++i) {
    double x1 = a + i * h;
    double x2 = x1 + h;
    double f1 = calculate(x1);
    double f2 = calculate(x2);
    integral += (f1 + f2) * h / 2.0;
  }

  return std::min(1.0, std::max(0.0, integral));
}

QPair<double, double> FDistribution::getSuggestedRange() const {
  // F 分布的建议范围
  double mode = (m_df1 - 2.0) * m_df2 / (m_df1 * (m_df2 + 2.0));
  if (mode <= 0)
    mode = 1.0;
  double max_x = mode + 3.0 * (1.0 + m_df1 / m_df2);
  return qMakePair(-0.1, max_x + 2.0);
}

double GammaDistribution::calculate(double x) const {
  if (x <= 0)
    return 0.0;

  double alpha = m_alpha;
  double beta = m_beta;

  double numerator =
      std::pow(beta, alpha) * std::pow(x, alpha - 1.0) * std::exp(-beta * x);
  double denominator = std::tgamma(alpha);

  return numerator / denominator;
}

double GammaDistribution::calculateCDF(double x) const {
  if (x <= 0)
    return 0.0;

  double alpha = m_alpha;
  double beta = m_beta;
  double scaled_x = beta * x;

  double gamma_alpha = std::tgamma(alpha);
  if (gamma_alpha == 0.0)
    return 1.0;

  const int steps = 10000;
  double integral = 0.0;
  double a = 0.0;
  double b = scaled_x;
  double h = (b - a) / steps;

  for (int i = 0; i < steps; ++i) {
    double x1 = a + i * h;
    double x2 = x1 + h;
    double f1 = (x1 > 0) ? std::pow(x1, alpha - 1.0) * std::exp(-x1) : 0.0;
    double f2 = (x2 > 0) ? std::pow(x2, alpha - 1.0) * std::exp(-x2) : 0.0;
    integral += (f1 + f2) * h / 2.0;
  }

  return std::min(1.0, std::max(0.0, integral / gamma_alpha));
}

QPair<double, double> GammaDistribution::getSuggestedRange() const {
  double mean = m_alpha / m_beta;
  double variance = m_alpha / (m_beta * m_beta);
  double std_dev = std::sqrt(variance);
  double max_x = mean + 4.0 * std_dev;
  return qMakePair(-0.5, max_x + 1.0);
}
