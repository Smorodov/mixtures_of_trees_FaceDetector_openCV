#include "Detect.h"

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
float score;
float Ix;
float Iy;
};

vector<Box> Detect(Mat& im,Model model,double thresh)
{
	const int BOXCACHESIZE=100000;
	vector<Box> boxes(BOXCACHESIZE);
	int cnt=0;

	Pyra pyra(im,model); // detect.m строка 13
	vector<vector<TComponent>> components;
	vector<vector<Mat>> filters;
	vector<Mat> resp;
	ModelComponents(model,pyra,components,filters,resp);

	return boxes;
}

// [components,filters,resp]  = modelcomponents(model,pyra);

void ModelComponents(Model& model,Pyra& pyra, vector<vector<TComponent>>& components,vector<vector<Mat>>& filters,vector<Mat>& resp)
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
		for(int j=0;j<filters[i].size();++j)
		{
			filters[i][j]=model.filters[i].w[j].clone(); // может и не нужно клонировать?
		}
	}


}