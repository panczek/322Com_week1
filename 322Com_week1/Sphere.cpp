#include "Sphere.h"
#include <math.h>

Sphere::Sphere(vec3 pos, float radius, vec3 colour) 
{
	
	this->position = pos;
	this->radius = radius;
	this->colour = colour;
	
}


bool Sphere::intersect(vec3 origin, vec3 direction, HitInfo& info)
{
	//info.boundBox = Box(vec3(this->position.x - this->radius, this->position.y - this->radius, this->position - this->radius),
		//vec3(this->position.x + this->radius, this->position.y + this->radius, this->position + this->radius));
	
	info.id = 1;
	vec3 l = this->position - origin;
	float target = dot(l, direction);// l.x* direction.x + l.y * direction.y + l.z * direction.z;
	//cout << direction.x << "___" << direction.y << "____" << direction.z << endl;

	//std::cout << target << std::endl;

	if (target < 0) {
		return false;
	}
	float s;
	float lenghL = dot(l, l);
	s = sqrt(lenghL - target * target);
	//std::cout << s << std::endl;
	if (s > this->radius) {
		return false;
	}
	float targetHC = sqrt(this->radius * this->radius - s * s);

	float t0 = target - targetHC;
	float t1 = target + targetHC;
	
	
	vec3 p = origin + t0 * direction;
	this->point = p;

	this->target = t0;


	info.colour = this->colour;
	info.point = this->point;
	info.t = this->target;
	info.position = this->position;
	info.normal = this->point - this->position;
	
	
	this->point = this->point + info.normal * (float)1e-4;
	info.normal = this->point - this->position;
	info.point = this->point;
	return true;
}

vec3 Sphere::getMin()
{
	//cout << "Sphere: " << this->position.x - this->radius << " ___ " << this->position.y - this->radius << " ___ " << this->position.z - this->radius << endl;
	return vec3(this->position.x - this->radius, this->position.y - this->radius, this->position.z - this->radius);
}

vec3 Sphere::getMax()
{
	return vec3(this->position.x + this->radius, this->position.y + this->radius, this->position.z + this->radius);
}

int Sphere::getId()
{
	return 1;
}




