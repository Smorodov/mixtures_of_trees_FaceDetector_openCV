#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "ModelComponent.h"
#include "ModelFilter.h"
#include "ModelDef.h"
using namespace cv;
using namespace std;

#pragma once
class Model
{
private:
	int ReadInt(string filename);
	float ReadFloat(string filename);
	// -------------------------------------------------------------------
	// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
	// -------------------------------------------------------------------
	void Read2DMat(ifstream& ifs,Mat& M);
	// -------------------------------------------------------------------
	// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
	// -------------------------------------------------------------------
	Mat Load2DMatrix(string filename);

public:
	string ModelDir;
	vector<ModelComponent> components;
	vector<ModelFilter> filters;
	vector<ModelDef> defs;
	cv::Size maxsize;
	size_t len;
	size_t interval;
	size_t sbin;
	double delta;
	double thresh;
	double obj;
	Model(string model_dir,size_t n_components,size_t n_filters,size_t n_defs);
	~Model();
};
