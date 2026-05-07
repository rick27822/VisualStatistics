#include "distributionmodel.h"
#include<cmath>
#include<algorithm>

double NormalDistribution::calculate(double x) const {
    double exponent = -0.5 * std::pow((x - m_mu) / m_sigma, 2);
    return (1.0 / (m_sigma * std::sqrt(2.0 * M_PI))) * std::exp(exponent);
}

QPair<double, double> NormalDistribution::getSuggestedRange() const {
    return qMakePair(m_mu - 5 * m_sigma, m_mu + 5 * m_sigma);
}
