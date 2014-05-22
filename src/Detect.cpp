#include "Detect.h"
#include <algorithm>    // std::random_shuffle

class TComponent
{
public:
	TComponent(){};
	~TComponent(){};
int defid;
int filterid;
int parent;
int sizy;
int sizx;
int filterI;
int defI;
Mat w;
float scale;
float starty;
float startx;
float step;
float level;
Mat	 score;
Mat Ix;
Mat Iy;
};

vector<int> randperm(int n_beg,int n_end)
{
	std::vector<int> ind;
	for (int i=0; i<n_end-n_beg; ++i)
	{
		ind.push_back(i+n_beg);
	}
	// ѕеремешаем элементы вектора c
	std::random_shuffle ( ind.begin(), ind.end() );
	return ind;
}


void findThres(Mat& src,float thres,vector<int>& Y,vector<int>& X)
{
	for (int i=0;i<src.rows;++i)
	{
		for (int j=0;j<src.cols;++j)
		{
			if(src.at<float>(i,j)>thres)
			{
				Y.push_back(i);
				X.push_back(j);
			}
		}
	}
}

vector<Box> Detect(Mat& im,Model model,double thresh)
{
	const int BOXCACHESIZE=100000;
	vector<Box> boxes(BOXCACHESIZE);
	int cnt=0;

	Pyra pyra(im,model); // detect.m строка 13
	vector<vector<TComponent>> components;
	vector<vector<Mat>> filters;
	vector<vector<Mat>> resp;
	ModelComponents(model,pyra,components,filters,resp);
	//cout << pyra.feat[27][1] << endl;
	//MATLAB: for c  = randperm(length(components))
	

	std::vector<int> ind_c=randperm(0,components.size());

	int c;
	int minlevel;
	std::vector<int> levels;
	int rlevel;
	vector<TComponent> parts;
	int numparts;
	for(int ic=0;ic<ind_c.size();ic++)
	{
		c= ind_c[ic];
		minlevel=model.interval; //+1;
		levels=randperm(minlevel,pyra.feat.size());
		for (int il=0;il<levels.size();il++)
		{
			rlevel=levels[il];
			parts=components[c];
			numparts=parts.size();
			for(int k=0;k<numparts;++k)
			{
				int f=parts[k].filterid-1;
				int level=rlevel-parts[k].scale*model.interval;
				if(resp[level].empty())
				{
					resp[level]=fconv(pyra.feat[level],filters,0,filters.size());
					//cout << resp[level][0] << endl;
				}
				parts[k].score=resp[level][f];//.clone(); // может и не нужно клонировать?
				parts[k].level=level;

			} // detect.m    line 36

			for (int k=numparts-1;k>0;--k)
			{
				TComponent child=parts[k];
				int par=child.parent;
				int Ny=parts[par].score.rows;
				int Nx=parts[par].score.cols;
				Mat msg;
				shiftdt(child.score,child.w.at<float>(0),child.w.at<float>(1),child.w.at<float>(2),child.w.at<float>(3),child.startx,child.starty,Nx,Ny,child.step,msg,parts[k].Ix,parts[k].Iy);
				parts[par].score+=msg;
			}

			Mat rscore;
			rscore=parts[0].score+parts[0].w.at<float>(0);
			vector<int> Y;
			vector<int> X;
			findThres(rscore,thresh,Y,X);

			if(!X.empty())
			{
				// XY=backtrack
			}



		}
	}


	return boxes;
}





// --------------------------------------------------------------
// [components,filters,resp]  = modelcomponents(model,pyra);
// --------------------------------------------------------------
void ModelComponents(Model& model,Pyra& pyra, vector<vector<TComponent>>& components,vector<vector<Mat>>& filters,vector<vector<Mat>>& resp)
{
	components.resize(model.components.size());
	for (int c=0;c<model.components.size();++c)
	{
		components[c].resize(model.components[c].Item.size());

		for (int k=0;k<model.components[c].Item.size();++k)
		{
			TComponent p;
			p.defid = model.components[c].Item[k].defid;
			p.filterid = model.components[c].Item[k].filterid;
			p.parent = model.components[c].Item[k].parent;

			ModelFilter x = model.filters[p.filterid-1];
			p.sizy=x.w[0].rows;
			p.sizx=x.w[0].cols;
			p.filterI=x.i;
			ModelDef xx=model.defs[p.defid-1];
			p.defI=xx.i;
			p.w=xx.w.clone();

			int par=p.parent;
			float ax  = xx.anchor.at<float>(0);
			float ay  = xx.anchor.at<float>(1);
			float ds  = xx.anchor.at<float>(2);

			if (par > 0)
			{
				p.scale = ds + components[c][par-1].scale;
			}
			else
			{
				p.scale = 0;
			}
			// amount of (virtual) padding to hallucinate
			int	step     = pow(2,ds);
			int virtpady = (step-1)*pyra.pady;
			int virtpadx = (step-1)*pyra.padx;
			// starting points (simulates additional padding at finer scales)
			p.starty = ay-virtpady;
			p.startx = ax-virtpadx;
			p.step   = step;
			p.level  = 0;
			p.score  = 0;
			p.Ix     = 0;
			p.Iy     = 0;
			components[c][k] = p;
		}
	}

	resp.resize(pyra.feat.size());
	filters.resize(model.filters.size());
	
	for (int i=0;i<filters.size();++i)
	{
		filters[i].resize(model.filters[i].w.size());
		for(int j=0;j<model.filters[i].w.size();++j)
		{
			filters[i][j]=model.filters[i].w[j];//.clone(); // может и не нужно клонировать?
		}
	}


}