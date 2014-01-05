#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "Model.h"

using namespace cv;
using namespace std;

#pragma once

vector<Mat> features(Mat& img,int sbin);
