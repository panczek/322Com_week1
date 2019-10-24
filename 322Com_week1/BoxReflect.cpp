#include "BoxReflect.h"
BoxReflect::BoxReflect(vec3 pmin, vec3 pmax, vec3 colour) {
	this->pMin = pmin;
	this->pMax = pmax;
	this->colour = colour;
}
bool BoxReflect::intersect(vec3 origin, vec3 direction, HitInfo& info)
{
	info.id = 3;
	vec3 t1;
	vec3 t2;
	if (direction.x) {
		t1.x = this->pMin.x - origin.x / direction.x;
		t2.x = this->pMax.x - origin.x / direction.x;
	}
	if (direction.y) {
		t1.y = this->pMin.x - origin.y / direction.y;
		t2.y = this->pMax.y - origin.y / direction.y;
	}
	if (direction.x) {
		t1.z = this->pMin.z - origin.z / direction.z;
		t2.z = this->pMax.z - origin.z / direction.z;
	}
	vec3 p1 = origin + direction * t1;

	info.point = p1;
	info.position = vec3((this->pMax.x + this->pMin.x) / 2.f, (this->pMax.y + this->pMin.y) / 2.f, (this->pMax.z + this->pMin.z) / 2.f);
	info.t = length(t1);
	info.normal = vec3(0.f, 0.f, 1.f);
	info.colour = this->colour;


	float epsilon = 0.001f;
	if (p1.x > (pMin.x - epsilon) && p1.x < (pMax.x + epsilon) &&
		p1.y >(pMin.y - epsilon) && p1.y < (pMax.y + epsilon) &&
		p1.z >(pMin.z - epsilon) && p1.z < (pMax.z + epsilon)) {
		return true;

	}
	else {
		return false;
	}
}
