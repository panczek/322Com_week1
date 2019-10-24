#pragma once

#include <string>
#include <glm-0.9.6.3/glm/glm.hpp>

#include <iostream>
using namespace glm;
using namespace std;
struct HitInfo
{
	vec3 point;
	float t;
	vec3 colour;
	bool hit;
	vec3 position;
	vec3 normal;
	int id;
};
class Shape
{
public:
	virtual bool intersect(vec3 origin, vec3 direction, HitInfo& info) = 0;
	virtual vec3 getMin();
	virtual vec3 getMax();
	virtual int getId() = 0;
		
};

