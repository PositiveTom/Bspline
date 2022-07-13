#include<iostream>
#include"src/BSpline.h"

int main()
{
	//double Points[2][4] = {
	//	{1.0, 4.3, 5.8, 10.6},
	//	{3.0, -3.0, 4.0, 0.7}
	//}

	Eigen::Matrix<double, 2, 5> cPoints;
	cPoints  << 100, 200, 300, 400, 500,
				300, 400, 100, 600, 300;
	cout << "cPoints:"<< endl << cPoints << endl;
	//	B������������Ҫ��һ������
	//	n+1 > k-1 -->  n+2 > k
	int n = 4;
	int k = 3;
	// flag 1: ����B����
	//	    2: ׼����B���� : �����յ���k���ظ��ȣ�k��k�׵�k
	//		3: �Ǿ���B����
	int flag = 2;

	Bspline* myspline = new Bspline(n, k, cPoints);
	
	myspline->BSplineCurve(flag);


	waitKey(0);
	return 0;
}



//	Eigen����ֵ����ʵ���ǳ�ʼ��
//	