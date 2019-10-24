#pragma once
#include "Shape.h"
class Sphere : public Shape
{
public:
	Sphere(vec3 pos, float radius, vec3 colour);
	vec3 position;
	float radius;
	vec3 colour;
	float target;
	vec3 point;
	bool intersect(vec3 origin, vec3 direction, HitInfo& info) override;
	vec3 getMin() override;
	vec3 getMax() override;
	int getId() override;
};

