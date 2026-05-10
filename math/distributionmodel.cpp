#include "distributionmodel.h"
#include<cmath>

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
double BinomialDistribution::calculate(double x) const{
    int k = std::round(x);
    if (k < 0 || k > n) return 0;
    if (p <= 0) return (k == 0) ? 1.0 : 0.0;
    if (p >= 1) return (k == n) ? 1.0 : 0.0;

    // 使用对数计算组合数公式防止溢出
    double logCnK = std::lgamma(n + 1) - std::lgamma(k + 1) - std::lgamma(n - k + 1);
    double logProb = logCnK + k * std::log(p) + (n - k) * std::log(1.0 - p);

    return std::exp(logProb);
}
QPair<double,double> BinomialDistribution::getSuggestedRange() const{
    return qMakePair(-1.0, double(n) + 1.0);
}
double BinomialDistribution::calculateCDF(double x) const{
    int k_max = std::floor(x);
    if (k_max < 0) return 0.0;
    if (k_max >= n) return 1.0;

    double sum = 0.0;
    for (int i = 0; i <= k_max; ++i) {
        sum += calculate(double(i));
    }
    return sum;
}
