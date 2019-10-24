#include "Plane.h"

Plane::Plane(vec3 pos, vec3 normal, vec3 colour)
{
	this->colour = colour;
	this->position = pos;
	this->normal = normalize(normal);
}

bool Plane::intersect(vec3 origin, vec3 direction, HitInfo& info)
{
	info.id = 0;
	float t = dot(origin - this->position, this->normal) / dot(direction, normal);
	vec3 p = origin + t * direction;
	this->target = t;

	//cout << p.x << " ___ " << p.y << " __ " << p.z << endl;

	info.t = t;
	info.point = p;
	info.normal = this->normal;
	int temp = (int)p.x;
	int tempz = (int)p.z;
	if ((temp % 3) == 0 || (tempz & 3) == 0) {
		info.colour = vec3(1.f, 0, 0);
	}
	else {
		info.colour = this->colour;
	}

	
	return(t > 0);

}

vec3 Plane::getMin()
{
	return vec3(-2000, this->position.y - 0.001, 20000);
}

vec3 Plane::getMax()
{
	return vec3(2000, this->position.y + 0.001, -2500);
}

int Plane::getId()
{
	return 0;
}


