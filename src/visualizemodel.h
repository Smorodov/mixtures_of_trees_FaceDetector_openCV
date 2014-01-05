#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Model.h"
using namespace cv;
using namespace std;
#pragma once

void visualizemodel(Model model,size_t comp_id_beg,size_t comp_id_end);
vector<Mat> foldHOG(vector<Mat>& w);
float maxw(vector<Mat>& w);
Mat HOGpicture(vector<Mat>& w,int bs);
