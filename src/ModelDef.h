#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#pragma once
class ModelDef
{
private:
	// -------------------------------------------------------------------
	// �������� ������. ��� �������������� � �������� �� ����� �������.
	// -------------------------------------------------------------------
	void Read2DMat(ifstream& ifs,Mat& M);
	// -------------------------------------------------------------------
	// �������� ������. ��� �������������� � �������� �� ����� �������.
	// -------------------------------------------------------------------
	Mat Load2DMatrix(string filename);
	int ReadInt(string filename);
public:
	std::string dir;
	Mat w;
	Mat anchor;
	int i;

	ModelDef(size_t n,string ModelDir);
	~ModelDef(void);
};