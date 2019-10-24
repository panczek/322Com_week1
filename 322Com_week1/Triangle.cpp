#include "Triangle.h"

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3, vec3 n1, vec3 n2, vec3 n3, vec3 colour)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->n1 = glm::normalize(n1);
	this->n2 = glm::normalize(n2);
	this->n3 = glm::normalize(n3);
	this->colour = colour;
}

bool Triangle::intersect(vec3 origin, vec3 direction, HitInfo& info)
{
	info.id = 2;
	
	vec3 e1 = this->p2 - this->p1;
	vec3 e2 = this->p3 - this->p1;

	float opt1 = dot(e1, cross(direction, e2));
	vec3 opt2 = origin - this->p1;

	float u = dot(opt2, cross(direction, e2)) / opt1;
	float v = dot(direction, cross((opt2), e1)) / opt1;
	float w = 1.0f - u - v;
	
	/*if (u < 0 || u > 1 || v < 0 || u + v > 1) {
		return false;
	}*/
		

	vec3 uVec = this->p2 - this->p1;
	vec3 vVec = this->p3 - this->p1;

	//w u v
	vec3 normal = normalize(w * this->n1 + u * this->n2 + v * this->n3); // one single= average of three normal vectors
	//vec3 normal = normalize(0.3333333f * (this->n1 + this->n2 + this->n3));
	if (dot(direction, normal) > 0) {
		normal *= -1.0f;
	}


	float t = dot(e2, cross(opt2, e1)) / opt1;
	this->target = t;
	if (t <= 0) {
		return false;
	}
	
	info.t = t;
	info.point = origin + t * direction;//this->p1 * u + this->p2 * v + this->p3 * w;
	info.normal = normal;
	info.colour = this->colour;
	//info.position = this->p1 * w + this->p2 * u + this->p3 * v; 


	if (u < 0 || u > 1) {
		return false;
	}
	else if (v < 0 || (u + v) > 1) {
		return false;
	}
	else {

		return true;
	}
}

vec3 Triangle::getMin()
{
	vec3 tempMin;
	tempMin = this->p1;
	if (this->p2.x < tempMin.x) {
		tempMin.x = this->p2.x;
	}
	if (this->p2.y < tempMin.y) {
		tempMin.y = this->p2.y;
	}
	if (this->p2.z < tempMin.z) {
		tempMin.z = this->p2.z;
	}
	if (this->p3.x < tempMin.x) {
		tempMin.x = this->p3.x;
	}
	if (this->p3.y < tempMin.y) {
		tempMin.y = this->p3.y;
	}
	if (this->p3.z < tempMin.z) {
		tempMin.z = this->p3.z;
	}
	return tempMin;
}

vec3 Triangle::getMax()
{
	vec3 tempMax;
	tempMax = this->p1;
	if (this->p2.x > tempMax.x) {
		tempMax.x = this->p2.x;
	}
	if (this->p2.y > tempMax.y) {
		tempMax.y = this->p2.y;
	}
	if (this->p2.z > tempMax.z) {
		tempMax.z = this->p2.z;
	}
	if (this->p3.x > tempMax.x) {
		tempMax.x = this->p3.x;
	}
	if (this->p3.y > tempMax.y) {
		tempMax.y = this->p3.y;
	}
	if (this->p3.z > tempMax.z) {
		tempMax.z = this->p3.z;
	}
	return tempMax;
}

int Triangle::getId()
{
	return 2;
}


