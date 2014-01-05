#include "visualizemodel.h"
// -------------------------------------------------------------------
// Визуализация загруженной модели
// Избавиться от динамического изменения размера изображения.
// -------------------------------------------------------------------
void visualizemodel(Model model,size_t comp_id_beg,size_t comp_id_end)
{
	int pad=2;
	int bs=20;
	int numparts;

	namedWindow("im");
	for (int i=comp_id_beg;i<comp_id_end;++i)
	{
		ModelComponent& c=model.components[i];
		numparts=c.Item.size();

		vector<int> Nmix(numparts,0);
		
		ModelComponent::item part=c.Item[0];
		vector<Mat> w=model.filters[part.filterid].w;
		w=foldHOG(w);
		float scale=maxw(w);
		Mat p = HOGpicture(w,bs);
		p/=scale;
		
		int top,bottom,left,right;
		top=bottom=left=right=pad;
		copyMakeBorder( p, p, top, bottom, left, right, BORDER_CONSTANT,cv::Scalar(0.5,0.5,0.5,0.5) );
		Mat im=p.clone();

		vector<cv::Point> startpoint(numparts);
		vector<int> partsize(numparts);
		startpoint[0]=cv::Point(0,0);
		partsize[0]=p.cols;

		for (int k=1;k<numparts;k++)
		{
			part=c.Item[k];
			int parent=c.Item[k].parent-1;
			w=model.filters[part.filterid-1].w;
			w=foldHOG(w);
			scale=maxw(w);
			p = HOGpicture(w,bs);
			p/=scale;
			copyMakeBorder( p, p, top, bottom, left, right, BORDER_CONSTANT,cv::Scalar(0.5,0.5,0.5,0.5) );
			ModelDef& def=model.defs[part.defid-1];
			float x1=(def.anchor.at<float>(0)-1)*bs+startpoint[parent].x;
			float y1=(def.anchor.at<float>(1)-1)*bs+startpoint[parent].y;

			int W=im.cols;
			int H=im.rows;
			
			int new_h=MAX(0,-y1)+im.rows;
			int new_w=MAX(0,-x1)+im.cols;

			Mat imnew=Mat::zeros( new_h, new_w,CV_32FC1);
			im.copyTo(imnew(cv::Rect(MAX(0,-x1),MAX(0,-y1),im.cols,im.rows)));
			im=imnew.clone();

			for (int ii=0;ii<numparts;++ii)
			{
				startpoint[ii]+=cv::Point(MAX(0,-x1),MAX(0,-y1));
			}

			x1=MAX(0,x1);
			y1=MAX(0,y1);
			float x2=x1+p.cols;
			float y2=y1+p.rows;

			startpoint[k]=cv::Point(x1,y1);

			imnew.release();
			imnew=Mat::zeros(MAX(y1+p.rows,im.rows),MAX(x1+p.cols,im.cols),CV_32FC1);
			im.copyTo(imnew(Rect(0,0,im.cols,im.rows)));
			im=imnew.clone();

			p(Rect(0,0,p.cols,p.rows)).copyTo(im(Rect(x1,y1,p.cols,p.rows)));
			partsize[k]=p.rows;
		}

		Mat im_color;
		cv::cvtColor(im,im_color,cv::COLOR_GRAY2BGR);

		for (int k=1;k<numparts;++k)
		{
			int parent=c.Item[k].parent-1;
			cv::Point s=startpoint[k]+cv::Point(0.5*partsize[k],0.5*partsize[k]);
			cv::Point end=startpoint[parent]+cv::Point(0.5*partsize[parent],0.5*partsize[parent]);
			cv::line(im_color,s,end,Scalar(0,0,1),2,CV_AA);
		}

		imshow("im",im_color);
		cv::waitKey(50);
	}
}

// -------------------------------------------------------------------
//
// -------------------------------------------------------------------
vector<Mat> foldHOG(vector<Mat>& w)
{
vector<Mat> res;
	for(int i=0;i<9;++i)
	{
		Mat m(w[i].size(),CV_32FC1);
		for(int j=0;j<m.rows;++j)
		{
			for(int k=0;k<m.cols;++k)
			{
				m.at<float>(j,k)=MAX(w[i].at<float>(j,k),0)+MAX(w[i+9].at<float>(j,k),0)+MAX(w[i+18].at<float>(j,k),0);
			}
		}
		res.push_back(m);
	}
return res;
}

// -------------------------------------------------------------------
//
// -------------------------------------------------------------------
Mat imrotate(Mat& source, double angle,int border=20)
{
	Mat bordered_source;
	int top,bottom,left,right;
	top=bottom=left=right=border;
	copyMakeBorder( source, bordered_source, top, bottom, left, right, BORDER_CONSTANT,cv::Scalar() );
	Point2f src_center((float)bordered_source.cols/2.0F, (float)bordered_source.rows/2.0F);
	Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	Mat dst;
	warpAffine(bordered_source, dst, rot_mat, bordered_source.size());  
	return dst;
}

// -------------------------------------------------------------------
//
// -------------------------------------------------------------------
Mat HOGpicture(vector<Mat>& w,int bs)
{
	Mat bim1=Mat::zeros(bs,bs,CV_32FC1);
	bim1.col(cvRound((float)bs/2.0))=1;
	vector<Mat> bim(9);
	bim[0]=bim1.clone();
	for (int i=1;i<9;++i)
	{
		bim[i]=imrotate(bim1,-(float)i*20.0,0);
	}
	Mat im=Mat::zeros(bs*w[0].rows,bs*w[0].cols,CV_32FC1);

	for (int i=0;i<w[0].rows;++i)
	{
		for (int j=0;j<w[1].cols;++j)
		{
			for (int k=0;k<9;++k)
			{
				Mat I=im(cv::Range(i*bs,(i+1)*bs),cv::Range(j*bs,(j+1)*bs));
				I=I+bim[k]*w[k].at<float>(i,j);
			}
		}
	}
	return im;
}

// -------------------------------------------------------------------
//
// -------------------------------------------------------------------
float maxw(vector<Mat>& w)
{
	float m=FLT_MIN;
	for(int i=0;i<w.size();++i)
	{
		for(int j=0;j<w[i].rows;++j)
		{
			for(int k=0;k<w[i].cols;++k)
			{
				if(m<w[i].at<float>(j,k)){m=w[i].at<float>(j,k);}
			}
		}
	}
	return m;
}