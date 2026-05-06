#include "distributionmodel.h"

double DistributionModel::normalPDF(double x,double mu,double sigma) {
    //防止除0错误，保证方差有效
    if(sigma<=0)
        return 0;
    //系数
    double coefficient = 1.0/(sigma*sqrt(2*M_PI));
    //指数
    double exponent = -0.5*pow((x-mu)/sigma,2);
    return coefficient*exp(exponent);
}
