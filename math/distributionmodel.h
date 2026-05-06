#ifndef DISTRIBUTIONMODEL_H
#define DISTRIBUTIONMODEL_H

#include<cmath>
#include<vector>

using namespace std;;

class DistributionModel
{
public:
    //正态分布密度函数
    static double normalPDF(double x,double mu,double sigma);
    //正态函数接口
    static void generateNormalData(vector<double> &xVec,vector<double> &yVec,double mu,double sigma,double start,double end,double points);

};

#endif // DISTRIBUTIONMODEL_H
