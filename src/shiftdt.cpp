#define INF 1E20
#include <math.h>
#include <sys/types.h>
#include <opencv2/opencv.hpp>
using namespace cv;

/*
* shiftdt.cc
* Generalized distance transforms based on Felzenswalb and Huttenlocher.
* This applies computes a min convolution of an arbitrary quadratic function ax^2 + bx
* This outputs results on an shifted, subsampled grid (useful for passing messages between variables in different domains)
*/

static inline int square(int x)
{
	return x*x;
}

// dt1d(source,destination_val,destination_ptr,source_step,source_length,
//      a,b,dest_shift,dest_length,dest_step)
void dt1d(float *src, float *dst, int *ptr, int step, int len, float a, float b, int dshift, int dlen, float dstep)
{
	int   *v = new int[len];
	float *z = new float[len+1];
	int k = 0;
	int q = 0;
	v[0] = 0;
	z[0] = -INF;
	z[1] = +INF;
	for (q = 1; q <= len-1; q++)
	{
		float s = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
		while (s <= z[k])
		{
			k--;
			s  = ((src[q*step] - src[v[k]*step]) - b*(q - v[k]) + a*(square(q) - square(v[k]))) / (2*a*(q-v[k]));
		}
		k++;
		v[k]   = q;
		z[k]   = s;
		z[k+1] = +INF;
	}
	k = 0;
	q = dshift;
	for (int i=0; i <= dlen-1; i++)
	{
		while (z[k+1] < q)
		{
			k++;
		}
		dst[i*step] = a*square(q-v[k]) + b*(q-v[k]) + src[v[k]*step];
		ptr[i*step] = v[k];
		q += dstep;
	}
	delete [] v;
	delete [] z;
}



// matlab entry point
// [M, Ix, Iy] = dt(vals, ax, bx, ay, by)
//void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
void shiftdt(Mat& vals,float ax,float bx,float ay,float by,int offx,int offy,int lenx,int leny,float step,Mat& M,Mat& Ix,Mat& Iy)
{

	// Read in deformation coefficients, negating to define a cost
	// Read in offsets for output grid, fixing MATLAB 0-1 indexing
	float *dvals = (float *)vals.data;//mxGetPr(prhs[0]);
	int sizx  = vals.cols;//mxGetN(prhs[0]);
	int sizy  = vals.rows;//mxGetM(prhs[0]);
	//	float ax = -mxGetScalar(prhs[1]);
	//	float bx = -mxGetScalar(prhs[2]);
	//	float ay = -mxGetScalar(prhs[3]);
	//	float by = -mxGetScalar(prhs[4]);

	offx--;
	offy--;
	//	int offx  = (int)mxGetScalar(prhs[5])-1;
	//	int offy  = (int)mxGetScalar(prhs[6])-1;
	//	int lenx  = (int)mxGetScalar(prhs[7]);
	//	int leny  = (int)mxGetScalar(prhs[8]);
	//	float step = mxGetScalar(prhs[9]);

	M=Mat::zeros(leny,lenx,CV_32FC1);
	Ix=Mat::zeros(leny,lenx,CV_32SC1);
	Iy=Mat::zeros(leny,lenx,CV_32SC1);
	//	mxArray  *mxM = mxCreateNumericMatrix(leny,lenx,mxDOUBLE_CLASS, mxREAL);
	//	mxArray *mxIy = mxCreateNumericMatrix(leny,lenx,mxINT32_CLASS, mxREAL);
	//	mxArray *mxIx = mxCreateNumericMatrix(leny,lenx,mxINT32_CLASS, mxREAL);

	float   *dM = (float *)M.data;
	int *dIy = (int *)Iy.data;
	int *dIx = (int *)Ix.data;

	float   *tmpM =  (float *)calloc(leny*sizx, sizeof(float));
	int *tmpIy = (int *)calloc(leny*sizx, sizeof(int));

	//printf("(%d,%d),(%d,%d),(%d,%d)\n",offx,offy,lenx,leny,sizx,sizy);
	// dt1d(source,destination_val,destination_ptr,source_step,source_length,
	//      a,b,dest_shift,dest_length,dest_step)
	for (int x = 0; x < sizx; x++)
	{
		dt1d(dvals+x*sizy, tmpM+x*leny, tmpIy+x*leny, 1, sizy, ay, by, offy, leny, step);
	}
	for (int y = 0; y < leny; y++)
	{
		dt1d(tmpM+y, dM+y, dIx+y, leny, sizx, ax, bx, offx, lenx, step);
	}
	// get argmins and adjust for matlab indexing from 1
	for (int x = 0; x < lenx; x++)
	{
		for (int y = 0; y < leny; y++)
		{
			int p = x*leny+y;
			dIy[p] = tmpIy[dIx[p]*leny+y]+1;
			dIx[p] = dIx[p]+1;
		}
	}
	free(tmpM);
	free(tmpIy);
	return;
}
