#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#pragma once
// -------------------------------------------------------------------
// Read components
// -------------------------------------------------------------------
class ModelComponent
{
private:
	// -------------------------------------------------------------------
	// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
	// -------------------------------------------------------------------
	void Read2DMat(ifstream& ifs,Mat& M);
	// -------------------------------------------------------------------
	// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
	// -------------------------------------------------------------------
	Mat Load2DMatrix(string filename);
public:
	std::string dir;
	ModelComponent::ModelComponent(size_t n,string ModelDir);

	ModelComponent::~ModelComponent();

	class item
	{
	public:
		size_t defid;
		size_t filterid;
		size_t parent;
		item(size_t d, size_t f,size_t p)
		{
			defid=d;
			filterid=f;
			parent=p;
		}
	};

	vector<item> Item;
	void read(size_t n);
};


