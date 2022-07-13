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
	//	B样条曲线最重要的一个性质
	//	n+1 > k-1 -->  n+2 > k
	int n = 4;
	int k = 3;
	// flag 1: 均匀B样条
	//	    2: 准均匀B样条 : 起点和终点有k个重复度，k是k阶的k
	//		3: 非均匀B样条
	int flag = 2;

	Bspline* myspline = new Bspline(n, k, cPoints);
	
	myspline->BSplineCurve(flag);


	waitKey(0);
	return 0;
}



//	Eigen矩阵赋值，其实就是初始化
//	