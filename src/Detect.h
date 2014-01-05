#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "Model.h"
#include "Pyra.h"
#include "Box.h"
using namespace cv;
using namespace std;

class TComponent;

vector<Box> Detect(Mat& im,Model model,double thresh);
void ModelComponents(Model& model,Pyra& pyra, vector<vector<TComponent>>& components,vector<vector<Mat>>& filters,vector<Mat>& resp);