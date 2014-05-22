#include "Features.h"


#define	round(x)	((x-floor(x))>0.5 ? ceil(x) : floor(x))

// small value, used to avoid division by zero
#define eps 0.0001

// unit vectors used to compute gradient orientation
float uu[9] = {1.0000, 
	0.9397, 
	0.7660, 
	0.500, 
	0.1736, 
	-0.1736, 
	-0.5000, 
	-0.7660, 
	-0.9397};
float vv[9] = {0.0000, 
	0.3420, 
	0.6428, 
	0.8660, 
	0.9848, 
	0.9848, 
	0.8660, 
	0.6428, 
	0.3420};

static inline float min(float x, float y) { return (x <= y ? x : y); }
static inline float max(float x, float y) { return (x <= y ? y : x); }

static inline int min(int x, int y) { return (x <= y ? x : y); }
static inline int max(int x, int y) { return (x <= y ? y : x); }



vector<Mat> features(Mat& img,int sbin)
{
	vector<Mat> featMat;
	float *im=new float[img.rows*img.cols*3];

	for (int i=0;i<img.rows;++i)
	{
		for (int j=0;j<img.cols;++j)
		{
			Vec3f val=img.at<Vec3f>(i,j);
			im[i+j*img.rows]=val[0];
			im[i+j*img.rows+img.rows*img.cols]=val[1];
			im[i+j*img.rows+img.rows*img.cols*2]=val[2];
		}
	}

	int dims[2]={img.rows,img.cols};

	// memory for caching orientation histograms & their norms
	int blocks[2];
	blocks[0] = (int)round((float)img.rows/(float)sbin);
	blocks[1] = (int)round((float)img.cols/(float)sbin);
	float *hist = (float *)calloc(blocks[0]*blocks[1]*18, sizeof(float));
	float *norm = (float *)calloc(blocks[0]*blocks[1], sizeof(float));

	int out[3];
	out[0]=max(blocks[0]-2,0);
	out[1]=max(blocks[1]-2,0);
	out[2]=27+4+1;

	size_t feat_len=out[0]*out[1]*out[2];
	float* feat=new float[feat_len];
	memset(feat,0,out[0]*out[1]*out[2]*sizeof(float));

	featMat.clear();
	featMat.resize(out[2],Mat::zeros(out[0],out[1],CV_32FC1)); 

	int visible[2];
	visible[0] = blocks[0]*sbin;
	visible[1] = blocks[1]*sbin;

	for (int x = 1; x < visible[1]-1; x++)
	{
		for (int y = 1; y < visible[0]-1; y++)
		{
			// first color channel
			float *s = im + min(x, dims[1]-2)*dims[0] + min(y, dims[0]-2);
			float dy = *(s+1) - *(s-1);
			float dx = *(s+dims[0]) - *(s-dims[0]);
			float v = dx*dx + dy*dy;
			// second color channel
			s += dims[0]*dims[1];
			float dy2 = *(s+1) - *(s-1);
			float dx2 = *(s+dims[0]) - *(s-dims[0]);
			float v2 = dx2*dx2 + dy2*dy2;
			// third color channel
			s += dims[0]*dims[1];
			float dy3 = *(s+1) - *(s-1);
			float dx3 = *(s+dims[0]) - *(s-dims[0]);
			float v3 = dx3*dx3 + dy3*dy3;
			// pick channel with strongest gradient
			if (v2 > v)
			{
				v = v2;
				dx = dx2;
				dy = dy2;
			}
			if (v3 > v)
			{
				v = v3;
				dx = dx3;
				dy = dy3;
			}
			// snap to one of 18 orientations
			float best_dot = 0;
			int best_o = 0;
			for (int o = 0; o < 9; o++)
			{
				float dot = uu[o]*dx + vv[o]*dy;
				if (dot > best_dot)
				{
					best_dot = dot;
					best_o = o;
				}
				else if (-dot > best_dot)
				{
					best_dot = -dot;
					best_o = o+9;
				}
			}
			// add to 4 histograms around pixel using linear interpolation
			float xp = ((float)x+0.5)/(float)sbin - 0.5;
			float yp = ((float)y+0.5)/(float)sbin - 0.5;
			int ixp = (int)floor(xp);
			int iyp = (int)floor(yp);
			float vx0 = xp-ixp;
			float vy0 = yp-iyp;
			float vx1 = 1.0-vx0;
			float vy1 = 1.0-vy0;
			v = sqrt(v);
			if (ixp >= 0 && iyp >= 0)
			{
				*(hist + ixp*blocks[0] + iyp + best_o*blocks[0]*blocks[1]) +=
					vx1*vy1*v;
			}
			if (ixp+1 < blocks[1] && iyp >= 0)
			{
				*(hist + (ixp+1)*blocks[0] + iyp + best_o*blocks[0]*blocks[1]) +=
					vx0*vy1*v;
			}
			if (ixp >= 0 && iyp+1 < blocks[0])
			{
				*(hist + ixp*blocks[0] + (iyp+1) + best_o*blocks[0]*blocks[1]) +=
					vx1*vy0*v;
			}
			if (ixp+1 < blocks[1] && iyp+1 < blocks[0])
			{
				*(hist + (ixp+1)*blocks[0] + (iyp+1) + best_o*blocks[0]*blocks[1]) +=
					vx0*vy0*v;
			}
		}
	}
	// compute energy in each block by summing over orientations
	for (int o = 0; o < 9; o++)
	{
		float *src1 = hist + o*blocks[0]*blocks[1];
		float *src2 = hist + (o+9)*blocks[0]*blocks[1];
		float *dst = norm;
		float *end = norm + blocks[1]*blocks[0];
		while (dst < end)
		{
			*(dst++) += (*src1 + *src2) * (*src1 + *src2);
			src1++;
			src2++;
		}
	}

	// compute features
	for (int x = 0; x < out[1]; x++)
	{
		for (int y = 0; y < out[0]; y++)
		{
			float *dst = (feat + x*out[0] + y);
			float *src, *p, n1, n2, n3, n4;
			p = norm + (x+1)*blocks[0] + y+1;
			n1 = 1.0 / sqrt(*p + *(p+1) + *(p+blocks[0]) + *(p+blocks[0]+1) + eps);
			p = norm + (x+1)*blocks[0] + y;
			n2 = 1.0 / sqrt(*p + *(p+1) + *(p+blocks[0]) + *(p+blocks[0]+1) + eps);
			p = norm + x*blocks[0] + y+1;
			n3 = 1.0 / sqrt(*p + *(p+1) + *(p+blocks[0]) + *(p+blocks[0]+1) + eps);
			p = norm + x*blocks[0] + y;
			n4 = 1.0 / sqrt(*p + *(p+1) + *(p+blocks[0]) + *(p+blocks[0]+1) + eps);
			float t1 = 0;
			float t2 = 0;
			float t3 = 0;
			float t4 = 0;
			// contrast-sensitive features
			src = hist + (x+1)*blocks[0] + (y+1);

			for (int o = 0; o < 18; o++)
			{
				float tmp=*src;
				float h1 = min(*src * n1, 0.2);
				float h2 = min(*src * n2, 0.2);
				float h3 = min(*src * n3, 0.2);
				float h4 = min(*src * n4, 0.2);
				*dst = 0.5 * (h1 + h2 + h3 + h4);
				t1 += h1;
				t2 += h2;
				t3 += h3;
				t4 += h4;
				dst += out[0]*out[1];
				src += blocks[0]*blocks[1];
			}
			// contrast-insensitive features
			src = hist + (x+1)*blocks[0] + (y+1);
			for (int o = 0; o < 9; o++)
			{
				float sum = *src + *(src + 9*blocks[0]*blocks[1]);
				float h1 = min(sum * n1, 0.2);
				float h2 = min(sum * n2, 0.2);
				float h3 = min(sum * n3, 0.2);
				float h4 = min(sum * n4, 0.2);
				*dst = 0.5 * (h1 + h2 + h3 + h4);
				dst += out[0]*out[1];
				src += blocks[0]*blocks[1];
			}
			// texture features
			*dst = 0.2357 * t1;
			dst += out[0]*out[1];
			*dst = 0.2357 * t2;
			dst += out[0]*out[1];
			*dst = 0.2357 * t3;
			dst += out[0]*out[1];
			*dst = 0.2357 * t4;
			// truncation feature
			dst += out[0]*out[1];

		}
	}
	delete[] hist;
	delete[] norm;
	delete[] im;

	int ind=0;
	for (int k=0;k<out[2];++k)
	{
		for (int j=0;j<out[1];++j)
		{
			for (int i=0;i<out[0];++i)
			{
				featMat[k].at<float>(i,j)=feat[/*k*out[0]*out[1]+j*out[0]+i*/ ind];
				++ind;
			}
		}
	}

	delete []feat;
	return featMat;
}