#include "Model.h"

Model::Model(string model_dir,size_t n_components,size_t n_filters,size_t n_defs)
{
	ModelDir=model_dir;//"..\\..\\Model\\";
	for (size_t i=0;i<n_components;++i)
	{
		components.push_back(ModelComponent(i+1,ModelDir));
	}
	for (size_t i=0;i<n_filters;++i)
	{
		filters.push_back(ModelFilter(i+1,ModelDir));
	}
	for (size_t i=0;i<n_defs;++i)
	{
		defs.push_back(ModelDef(i+1,ModelDir));
	}
	string dir=ModelDir+"MaxSize\\";
	string fname="maxsize";
	string ext=".txt";
	string path=dir+fname+ext;
	Mat s=Load2DMatrix(path);
	maxsize.width=s.at<int>(0);
	maxsize.height=s.at<int>(1);

	dir=ModelDir+"Len\\";
	fname="len";
	path=dir+fname+ext;
	len=ReadInt(path);


	dir=ModelDir+"Interval\\";
	fname="interval";
	path=dir+fname+ext;
	interval=ReadInt(path);

	dir=ModelDir+"Sbin\\";
	fname="sbin";
	path=dir+fname+ext;
	sbin=ReadInt(path);

	dir=ModelDir+"Delta\\";
	fname="delta";
	path=dir+fname+ext;
	delta=ReadFloat(path);

	dir=ModelDir+"Thresh\\";
	fname="thresh";
	path=dir+fname+ext;
	thresh=ReadFloat(path);

	dir=ModelDir+"Obj\\";
	fname="obj";
	path=dir+fname+ext;
	obj=ReadFloat(path);
}
int Model::ReadInt(string filename)
{
	int I;
	ifstream ifs(filename);
	if (ifs.is_open())
	{
		ifs >> I;
		ifs.close();
	}
	return I;
}

float Model::ReadFloat(string filename)
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
// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
void Model::Read2DMat(ifstream& ifs,Mat& M)
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
Mat Model::Load2DMatrix(string filename)
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
Model::~Model()
{
	components.clear();
}
