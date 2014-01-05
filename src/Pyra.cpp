#include "Pyra.h"
#include "Features.h"
using namespace cv;
using namespace std;

Pyra::Pyra()
{

}

Pyra::Pyra(Mat& im,Model model)
{
	interval=model.interval;
	int sbin=model.sbin;
	padx=MAX(model.maxsize.width-1-1,0);
	pady=MAX(model.maxsize.height-1-1,0);
	float sc=pow(2.0f,1.0f/(float)interval);

	float max_scale=1.0f+floor(log(MIN(im.rows,im.cols)/(5.0f*(float)sbin))/log(sc));
	feat.resize(max_scale+interval);
	scale.resize(max_scale+interval);

	Mat scaled;
	for (int i=0;i<interval;++i)
	{
		float w=im.cols;
		float h=im.rows;
		w=w/pow(sc,i);
		h=h/pow(sc,i);
		resize(im,scaled,Size(w,h));
		feat[i]=features(scaled,sbin/2);
		scale[i]=2.0/pow(sc,i);
		feat[i+interval]=features(scaled,sbin);
		scale[i+interval]=1.0/pow(sc,i);
		for (int j=i+interval;j<max_scale;j+=interval)
		{
			resize(scaled,scaled,Size(scaled.cols/2,scaled.rows/2));
			feat[j+interval]=features(scaled,sbin);
			scale[j+interval]=0.5*scale[j];
		}
	}

	int top,bottom,left,right;

	top=bottom=padx+1;
	left=right=pady+1;

	for (int i=0;i<feat.size();++i)
	{
		int e=feat[i].size()-1;
		for (int j=0;j<feat.size()-1;++j)
		{
			copyMakeBorder( feat[i][j], feat[i][j], top, bottom, left, right, BORDER_CONSTANT,cv::Scalar(0,0,0,0) );
		}
		copyMakeBorder( feat[i][e], feat[i][e], top, bottom, left, right, BORDER_CONSTANT,cv::Scalar(1,1,1,1) );
	}

	for (int i=0;i<scale.size();++i)
	{
		scale[i]    = (float)model.sbin/(float)scale[i];
	}

	imy = im.rows;
	imx = im.cols;
	pady = pady;
	padx = padx;
}

Pyra::~Pyra()
{

}