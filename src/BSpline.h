#ifndef BSPLINE_H
#define BSPLINE_H

#include<iostream>
#include<opencv2/opencv.hpp>
#include<Eigen\Dense>
#include<vector>

using namespace std;
using namespace cv;

class Bspline
{
public:
	Bspline(int n, int k, Eigen::Matrix<double, 2, Eigen::Dynamic> cPoints);

	static vector<double> linspace(double begin, double end, int points);
	static vector<double> linspace(double begin, double end, double interval);

	void BSplineCurve(int flag);

	double BaseFunction(int i, int k, vector<double> &knotvector, double t_value);


public:
	//	控制点
	Eigen::Matrix2Xd cPoints;
	//	n = 控制点的个数 - 1 
	int n;
	//	B样条基函数的阶数 k = 次数 + 1
	int k;
};




#endif


//	手写Bspline曲线的过程中，碰到的一些必须面对的问题
//	eigen库运算，opencv库进行绘图运算，虽然opencv确实有矩阵运算功能，但是不要妄图去用opencv的矩阵运算去做复杂的功能
//	1.直接用矩阵存储点集吧 ，这里使用  Eigen::MatrixXd  
//		1.1.所有的matrix和vector都是matrix类
//		1.2.动态矩阵，意味着，当作函数形参时，这部分的大小是可变的，不是固定的