#include "ModelFilter.h"


ModelFilter::ModelFilter(size_t n,string ModelDir)
{
	dir=ModelDir+"Filters\\";

	string fname="w";
	string ext=".txt";
	char buf[1024];
	sprintf(buf,"%d",n);
	string num(buf);
	string path=dir+fname+num+ext;

	w=Load3DMatrix(path);

	fname="i";
	path=dir+fname+num+ext;
	i=ReadInt(path);
}

// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
void ModelFilter::Read3DMat(ifstream& ifs,vector<Mat>& M)
{
	int rows;
	int cols;
	int nmat;
	ifs >> rows;
	char comma;
	ifs >> comma;
	ifs >> cols;
	ifs >> comma;
	ifs >> nmat;

	M.clear();
	for(int j=0;j<nmat;++j)
	{
		M.push_back(Mat(rows,cols,CV_32FC1));
	}
	
	for(int j=0;j<rows;j++)
	{
		for(int k=0;k<cols*nmat;k++)
		{
			int ind=k/cols;
			ifs >> M[ind].at<float>(j,k%cols);
			if(k!=cols*nmat-1){ifs >> comma;}
		}
	}
}
// -------------------------------------------------------------------
// Загрузка матриц. Для взаимодействия с матлабом во время отладки.
// -------------------------------------------------------------------
vector<Mat> ModelFilter::Load3DMatrix(string filename)
{
	vector<Mat> M;
	ifstream ifs(filename);
	if (ifs.is_open())
	{
		Read3DMat(ifs,M);
		ifs.close();
	}
	return M;
}

int ModelFilter::ReadInt(string filename)
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

ModelFilter::~ModelFilter(void)
{
	w.clear();
}
