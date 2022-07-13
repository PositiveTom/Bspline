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
	//	���Ƶ�
	Eigen::Matrix2Xd cPoints;
	//	n = ���Ƶ�ĸ��� - 1 
	int n;
	//	B�����������Ľ��� k = ���� + 1
	int k;
};




#endif


//	��дBspline���ߵĹ����У�������һЩ������Ե�����
//	eigen�����㣬opencv����л�ͼ���㣬��Ȼopencvȷʵ�о������㹦�ܣ����ǲ�Ҫ��ͼȥ��opencv�ľ�������ȥ�����ӵĹ���
//	1.ֱ���þ���洢�㼯�� ������ʹ��  Eigen::MatrixXd  
//		1.1.���е�matrix��vector����matrix��
//		1.2.��̬������ζ�ţ����������β�ʱ���ⲿ�ֵĴ�С�ǿɱ�ģ����ǹ̶���