#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;

#pragma once

class Box
{
public:
	float s;
	float c;
	Mat xy;
	float level;
	Box(void);
	~Box(void);
};

