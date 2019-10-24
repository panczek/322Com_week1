#include "Box.h"



Box::Box()
{
}

Box::Box(vec3 pmin, vec3 pmax)
{
	this->pMax = pmax;
	this->pMin = pmin;
}

bool Box::intersect(vec3 origin, vec3 direction)
{

	//intersection method from
	//https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

	float xMin = (this->pMin.x - origin.x) / direction.x;
	float xMax = (this->pMax.x - origin.x) / direction.x;

	if (xMin > xMax) {
		swap(xMin, xMax);
	} 

	float yMin = (this->pMin.y - origin.y) / direction.y;
	float yMax = (this->pMax.y - origin.y) / direction.y;

	if (yMin > yMax) {
		swap(yMin, yMax);
	} 
	if ((xMin > yMax) || (yMin > xMax)) {
		return false;
	}
	if (yMin > xMin) {
		xMin = yMin;
	}
	if (yMax < xMax) {
		xMax = yMax;
	}	

	float zMin = (this->pMin.z - origin.z) / direction.z;
	float zMax = (this->pMax.z - origin.z) / direction.z;

	if (zMin > zMax) {
		swap(zMin, zMax);
	}
	if ((xMin > zMax) || (zMin > xMax)) {
		return false;
	}
	if (zMin > xMin) {
		xMin = zMin;
	}
	if (zMax < xMax) {
		xMax = zMax;
	}	
	this->t = length(vec3(xMin, yMin, zMin));

	return true;
		

}

void Box::findingPoints()
{
	Box tempBox;
	tempBox.pMin = this->boxShapes[0]->getMin();
	tempBox.pMax = this->boxShapes[0]->getMax();
	for (int i = 1; i < this->boxShapes.size(); i++) {
		vec3 tempMax = this->boxShapes[i]->getMax();
		vec3 tempMin = this->boxShapes[i]->getMin();
		if (tempBox.pMin.x > tempMin.x) {
			tempBox.pMin.x = tempMin.x;
		}
		if (tempBox.pMin.y > tempMin.y) {
			tempBox.pMin.y = tempMin.y;
		}
		if (tempBox.pMin.z > tempMin.z) {
			tempBox.pMin.z = tempMin.z;
		}
		if (tempBox.pMax.x < tempMax.x) {
			tempBox.pMax.x = tempMax.x;
		}
		if (tempBox.pMax.y < tempMax.y) {
			tempBox.pMax.y = tempMax.y;
		}
		if (tempBox.pMax.z < tempMax.z) {
			tempBox.pMax.z = tempMax.z;
		}
	}
	this->pMin = tempBox.pMin;
	this->pMax = tempBox.pMax;
	
	//cout << this->pMin.x << " ___ " << this->pMin.y << " ___ " << this->pMin.z << " ___ " <<
		//this->pMax.x << " ___ " << this->pMax.y << " ___ " << this->pMax.z << endl;

	
}
