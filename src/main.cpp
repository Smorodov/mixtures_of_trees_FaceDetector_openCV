#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Model.h"
#include "visualizemodel.h"
#include "Detect.h"
using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	// Загрузка модели
	Model model("..\\..\\Model\\",13,146,710);
	// Визуализация модели
	visualizemodel(model,0,13);

	model.interval=5;
	model.thresh=MIN(-0.65,model.thresh);
	// Загрузка изображения с лицами
	Mat im=imread("..\\..\\images\\1.jpg",1);
	// Работаем с плавающей точкой
	im.convertTo(im,CV_32FC3);

	// Детект
	vector<Box> boxes = Detect(im,model,model.thresh);

	return 0;
}
