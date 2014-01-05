#include "ModelComponent.h"

ModelComponent::ModelComponent(size_t n,string ModelDir)
{
	dir=ModelDir+"Components\\";
	read(n);
}

ModelComponent::ModelComponent()
{

}

// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
void ModelComponent::Read2DMat(ifstream& ifs,Mat& M)
{
	int rows;
	int cols;
	ifs >> rows;
	char comma;
	ifs >> comma;
	ifs >> cols;
	M=Mat(rows,cols,CV_32SC1);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			ifs >> M.at<int>(i,j);
			if(j!=cols-1){ifs >> comma;}
		}
	}
}
// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
Mat ModelComponent::Load2DMatrix(string filename)
{
	Mat M;
	ifstream ifs(filename);
	if (ifs.is_open())
	{
		Read2DMat(ifs,M);
		ifs.close();
	}
	return M;
}

void ModelComponent::read(size_t n)
{
	Mat m;
	string fname="component";
	string ext=".txt";
	char buf[1024];
	sprintf(buf,"%d",n);
	string num(buf);
	string path=dir+fname+num+ext;
	m=Load2DMatrix(path);

	for (size_t i=0;i<m.rows;++i)
	{
		Item.push_back( item(m.at<int>(i,0),m.at<int>(i,1),m.at<int>(i,2)) );
	}
}

ModelComponent::~ModelComponent(void)
{
	Item.clear();
}
