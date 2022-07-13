#include"BSpline.h"


Bspline::Bspline(int n, int k, Eigen::Matrix<double, 2, Eigen::Dynamic> cPoints)
{
	this->cPoints = cPoints;
	this->k = k;
	this->n = n;
}

vector<double> Bspline::linspace(double begin, double end, int points)
{
	vector<double> result;
	
	double scale = (end - begin)/(double(points) - 1.0);
	for (int i = 0; i < points; i++)
	{
		double temp = begin + scale * i;
		result.push_back(temp);
	}
	return result;
}

vector<double> Bspline::linspace(double begin, double end, double interval)
{
	vector<double> result;
	double temp = begin;
	for (; temp <= end; )
	{
		result.push_back(temp);
		temp += interval;
	}
	return result;
}

//	各种B样条的区别是否仅仅在于节点向量不同，是的，仅仅是节点向量不同

void Bspline::BSplineCurve(int flag)
{
	//	绘图，一旦涉及到图片，像素就是整型的了
	namedWindow("UniformBSpline", WINDOW_NORMAL);
	Mat figure(640, 640, CV_8UC3, Scalar(255, 255, 255));

	//	产生节点向量  个数等于  n+k+1
	vector<double> knotvector;
	switch (flag) 
	{
		case 1:
			knotvector = linspace(0, 1, this->k + this->n + 1);
			break;
		case 2:
			vector<double> temp(k-1, 0);
			vector<double> temp3(k - 1, 1);
			knotvector.insert(knotvector.end(), temp.begin(), temp.end());
			if (n - k + 3 > 1)
			{
				vector<double> temp2 = linspace(0, 1, n - k + 3);
				knotvector.insert(knotvector.end(), temp2.begin(), temp2.end());
			}
			knotvector.insert(knotvector.end(), temp3.begin(), temp3.end());
			break;
	}

	//	输出节点向量
	cout << "节点向量为:" << endl;
	vector<double>::iterator it = knotvector.begin();
	for (; it != knotvector.end(); it++)
	{
		cout << *it << " ";
		if (it == knotvector.end() - 1)
		{
			cout << endl;
		}
	}

	//	B样条基函数
	Eigen::Matrix<double, Eigen::Dynamic , 1 > Nik;
	int cols = cPoints.cols();
	Nik.resize(cols, 1);
	//	B样条基函数的定义区间
	double tk_1 = knotvector[k - 1];
	double tn_plus_1 = knotvector[n + 1];

	cout << endl;
	printf("%d阶B样条曲线的定义区间为[%f,%f]\n", k, tk_1, tn_plus_1);
	cout << endl;

	//	在定义的区间上一个值一个值算出来
	vector<double> t_values = linspace(tk_1, tn_plus_1, 0.001);
	vector<double>::iterator it2 = t_values.begin();
	for (; it2 != t_values.end();)
	{
		//	总共有n+1个样条基函数，每个样条基函数又由样条基函数递推得来
		for (int i = 0; i < n + 1; i++)
		{
			Nik(i, 0) = BaseFunction(i, k, knotvector, *it2);
			//cout << "result " << result << endl;
		}
		Eigen::Matrix<double, 2, 1> point = cPoints * Nik;
		circle(figure, Point(int(point(0,0)), int(point(1,0))), 2, Scalar(255,0,0), -1);
		it2++;
	}

	//	画点
	for (int i = 0; i < n + 1; i++)
	{
		Point temp;
		temp.x = int(cPoints(0, i));
		temp.y = int(cPoints(1, i));
		circle(figure, temp, 5, Scalar(0, 10, 0), -1);
	}

	//	连线
	for (int i = 0; i < n; i++)
	{
		Point temp1, temp2;
		temp1.x = int(cPoints(0, i));
		temp1.y = int(cPoints(1, i));
		temp2.x = int(cPoints(0, i+1));
		temp2.y = int(cPoints(1, i + 1));
		line(figure, temp1, temp2, Scalar(0, 0, 255), 2);
	}

	imshow("UniformBSpline", figure);
}

//	B样条基函数
double Bspline::BaseFunction(int i, int k, vector<double>& knotvector, double t_value)
{
	//	如果是1阶，0次
	if (k == 1)
	{
		//	Ni1 的定义区间在 t_i t_i+1
		//	如果在其定义的区间上 [ t_i, t_i+1 ]
		if (t_value >= knotvector[i] && t_value <= knotvector[i + 1])
		{
			return 1.0;
		}
		else
		{
			return 0.0;
		}
	}
	else
	{
		//	利用 0/0 = 0的性质
		double Length_i = knotvector[i + k - 1] - knotvector[i];
		double Length_i_1 = knotvector[i + k] - knotvector[i + 1];
		if (Length_i == 0)
		{
			Length_i = 1.0;
		}
		if (Length_i_1 == 0)
		{
			Length_i_1 = 1.0;
		}
		//	递推公式
		double result = (t_value - knotvector[i]) / Length_i * BaseFunction(i, k - 1, knotvector, t_value) +
			(knotvector[i + k] - t_value) / Length_i_1 * BaseFunction(i+1, k-1, knotvector, t_value);
		return result;
	}
}

//	1.均匀B样条曲线中，在生成节点向量的过程中，需要产生指定数量的等间隔的节点向量
//		c++似乎没有相关函数可以实现，只能使用linspace了
//	2.stl容器的push_back和insert方法
//	3.常量不能传递给引用类型的形参
//	4.stl容器输出元素，得需要用data方法 data方法返回的是只想存储位置的指针
//	5.输出vector容器只能用 迭代器，或者索引方法
//	6.尽量用 (double(points) - 1.0) 而不用 double(points - 1.0)
//	7.Mat创建指定大小，指定元素,这里就是Mat的初始化了
//	8.c++不支持同时赋多个值  类似于 a,b = 10, 20
//	9.迭代器不能重定义
//	10.嵌套函数无缘无故崩掉了
//	11.Eigen的动态矩阵得先声明维数之后，才能用索引访问元素，使用resize
//	12.vector的拼接  https://blog.csdn.net/Vichael_Chan/article/details/100516242?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-100516242-blog-82950913.pc_relevant_multi_platform_whitelistv1&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-100516242-blog-82950913.pc_relevant_multi_platform_whitelistv1&utm_relevant_index=1
//		insert函数	