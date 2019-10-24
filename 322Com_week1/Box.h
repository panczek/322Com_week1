#pragma once
#include "Sphere.h"
#include <vector>


struct boxInfo
{
	vec3 pMIn;
	vec3 pMax;
	vec3 t1;
	vec3 t2;
	vec3 p1;
};

class Box
{
public:
	Box();
	Box(vec3 pmin, vec3 pmax);
	vec3 pMin;
	vec3 pMax;
	bool intersect(vec3 origin, vec3 direction);
	vector <Shape*> boxShapes;
	vector<Box> insideBoxes;
	void findingPoints();
	float t;
};



