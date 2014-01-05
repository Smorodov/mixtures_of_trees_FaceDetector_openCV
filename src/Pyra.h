#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "Model.h"

using namespace cv;
using namespace std;

#pragma once

typedef vector<Mat> Feat;

class Pyra
{
public:
	vector<Feat> feat; // ������ 3D ������
	vector<double> scale;
	int interval;
	int imy;
	int imx;
	int pady;
	int padx;
	Pyra();
	Pyra(Mat& im,Model model);
	~Pyra();
};