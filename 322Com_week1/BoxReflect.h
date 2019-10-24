#pragma once
#include "Shape.h"
class BoxReflect : public Shape
{
public:
	BoxReflect(vec3 pmin, vec3 pmax, vec3 colour);
	vec3 pMin;
	vec3 pMax;
	vec3 colour;
	bool intersect(vec3 origin, vec3 direction, HitInfo& info) override;
};

