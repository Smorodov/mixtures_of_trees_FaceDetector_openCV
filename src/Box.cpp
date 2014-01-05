#include "Box.h"


Box::Box(void)
{
	s=0;
	c=0;
	level=0;
}


Box::~Box(void)
{
	xy.release();
}
