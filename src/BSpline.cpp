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

//	����B�����������Ƿ�������ڽڵ�������ͬ���ǵģ������ǽڵ�������ͬ

void Bspline::BSplineCurve(int flag)
{
	//	��ͼ��һ���漰��ͼƬ�����ؾ������͵���
	namedWindow("UniformBSpline", WINDOW_NORMAL);
	Mat figure(640, 640, CV_8UC3, Scalar(255, 255, 255));

	//	�����ڵ�����  ��������  n+k+1
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

	//	����ڵ�����
	cout << "�ڵ�����Ϊ:" << endl;
	vector<double>::iterator it = knotvector.begin();
	for (; it != knotvector.end(); it++)
	{
		cout << *it << " ";
		if (it == knotvector.end() - 1)
		{
			cout << endl;
		}
	}

	//	B����������
	Eigen::Matrix<double, Eigen::Dynamic , 1 > Nik;
	int cols = cPoints.cols();
	Nik.resize(cols, 1);
	//	B�����������Ķ�������
	double tk_1 = knotvector[k - 1];
	double tn_plus_1 = knotvector[n + 1];

	cout << endl;
	printf("%d��B�������ߵĶ�������Ϊ[%f,%f]\n", k, tk_1, tn_plus_1);
	cout << endl;

	//	�ڶ����������һ��ֵһ��ֵ�����
	vector<double> t_values = linspace(tk_1, tn_plus_1, 0.001);
	vector<double>::iterator it2 = t_values.begin();
	for (; it2 != t_values.end();)
	{
		//	�ܹ���n+1��������������ÿ�����������������������������Ƶ���
		for (int i = 0; i < n + 1; i++)
		{
			Nik(i, 0) = BaseFunction(i, k, knotvector, *it2);
			//cout << "result " << result << endl;
		}
		Eigen::Matrix<double, 2, 1> point = cPoints * Nik;
		circle(figure, Point(int(point(0,0)), int(point(1,0))), 2, Scalar(255,0,0), -1);
		it2++;
	}

	//	����
	for (int i = 0; i < n + 1; i++)
	{
		Point temp;
		temp.x = int(cPoints(0, i));
		temp.y = int(cPoints(1, i));
		circle(figure, temp, 5, Scalar(0, 10, 0), -1);
	}

	//	����
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

//	B����������
double Bspline::BaseFunction(int i, int k, vector<double>& knotvector, double t_value)
{
	//	�����1�ף�0��
	if (k == 1)
	{
		//	Ni1 �Ķ��������� t_i t_i+1
		//	������䶨��������� [ t_i, t_i+1 ]
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
		//	���� 0/0 = 0������
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
		//	���ƹ�ʽ
		double result = (t_value - knotvector[i]) / Length_i * BaseFunction(i, k - 1, knotvector, t_value) +
			(knotvector[i + k] - t_value) / Length_i_1 * BaseFunction(i+1, k-1, knotvector, t_value);
		return result;
	}
}

//	1.����B���������У������ɽڵ������Ĺ����У���Ҫ����ָ�������ĵȼ���Ľڵ�����
//		c++�ƺ�û����غ�������ʵ�֣�ֻ��ʹ��linspace��
//	2.stl������push_back��insert����
//	3.�������ܴ��ݸ��������͵��β�
//	4.stl�������Ԫ�أ�����Ҫ��data���� data�������ص���ֻ��洢λ�õ�ָ��
//	5.���vector����ֻ���� ��������������������
//	6.������ (double(points) - 1.0) ������ double(points - 1.0)
//	7.Mat����ָ����С��ָ��Ԫ��,�������Mat�ĳ�ʼ����
//	8.c++��֧��ͬʱ�����ֵ  ������ a,b = 10, 20
//	9.�����������ض���
//	10.Ƕ�׺�����Ե�޹ʱ�����
//	11.Eigen�Ķ�̬�����������ά��֮�󣬲�������������Ԫ�أ�ʹ��resize
//	12.vector��ƴ��  https://blog.csdn.net/Vichael_Chan/article/details/100516242?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-100516242-blog-82950913.pc_relevant_multi_platform_whitelistv1&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1-100516242-blog-82950913.pc_relevant_multi_platform_whitelistv1&utm_relevant_index=1
//		insert����	