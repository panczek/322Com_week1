#pragma once
#include "Shape.h"
class Triangle : public Shape
{
public:
	Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 n1, vec3 n2, vec3 n3, vec3 colour);
	vec3 p1;
	vec3 p2;
	vec3 p3;
	vec3 n1;
	vec3 n2;
	vec3 n3;
	float target;
	vec3 colour;
	bool intersect(vec3 origin, vec3 direction, HitInfo& info) override;
	vec3 getMin() override;
	vec3 getMax() override;
	int getId() override;
};

