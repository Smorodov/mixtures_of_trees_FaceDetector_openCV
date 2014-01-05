#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Model.h"
#include "visualizemodel.h"
#include "Pyra.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Model model("..\\..\\Model\\",13,146,710);
	
	visualizemodel(model,0,13);
	model.interval=5;
	model.thresh=MIN(-0.65,model.thresh);
	
	Mat im=imread("..\\..\\images\\1.jpg",1);
	im.convertTo(im,CV_32FC3);
	Pyra pyra(im,model); // detect.m строка 13

	return 0;
}
