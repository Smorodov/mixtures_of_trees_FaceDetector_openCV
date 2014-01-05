#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#pragma once
class ModelFilter
{
private:
	// -------------------------------------------------------------------
	// �������� ������. ��� �������������� � �������� �� ����� �������.
	// -------------------------------------------------------------------
	void Read3DMat(ifstream& ifs,vector<Mat>& M);
	// -------------------------------------------------------------------
	// �������� ������. ��� �������������� � �������� �� ����� �������.
	// -------------------------------------------------------------------
	vector<Mat> Load3DMatrix(string filename);

	int ReadInt(string filename);

public:
	std::string dir;
	vector<Mat> w;
	int i;
	ModelFilter(size_t n,string ModelDir);
	~ModelFilter(void);
};

