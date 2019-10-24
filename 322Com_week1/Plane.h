#pragma once
#include "Shape.h"
class Plane : public Shape
{
public:

	Plane(vec3 pos, vec3 normal, vec3 colour);
	vec3 colour;
	vec3 position;
	vec3 normal;
	float target;
	bool intersect(vec3 origin, vec3 direction, HitInfo& info) override;
	vec3 getMin() override;
	vec3 getMax() override;
	int getId() override;
};

