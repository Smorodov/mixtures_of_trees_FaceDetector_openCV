#include "ModelDef.h"


ModelDef::ModelDef(size_t n,string ModelDir)
{
	dir=ModelDir+"Defs\\";
	string fname="w";
	string ext=".txt";
	char buf[1024];
	sprintf(buf,"%d",n);
	string num(buf);
	string path=dir+fname+num+ext;
	w=Load2DMatrix(path);
	fname="anchor";
	path=dir+fname+num+ext;
	anchor=Load2DMatrix(path);
	fname="i";
	path=dir+fname+num+ext;
	i=ReadInt(path);
}
// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
void ModelDef::Read2DMat(ifstream& ifs,Mat& M)
{
	int rows;
	int cols;
	ifs >> rows;
	char comma;
	ifs >> comma;
	ifs >> cols;
	M=Mat(rows,cols,CV_32FC1);
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
		{
			ifs >> M.at<float>(i,j);
			if(j!=cols-1){ifs >> comma;}
		}
	}
}
// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
Mat ModelDef::Load2DMatrix(string filename)
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

int ModelDef::ReadInt(string filename)
{
	float I;
	ifstream ifs(filename);
	if (ifs.is_open())
	{
		ifs >> I;
		ifs.close();
	}
	return I;
}

ModelDef::~ModelDef(void)
{

}