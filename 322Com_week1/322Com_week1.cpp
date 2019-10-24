// 322Com_week1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <glm-0.9.6.3/glm/glm.hpp>
#include "objloader.h"
#include <typeinfo>

#include <SDL2-2.0.10/include/SDL.h>
#include <stdlib.h>


#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BoxReflect.h"
#include "Box.h"


#include <iterator>
#include <thread>
#include <cstdlib>
#include <ctime>
#define WINDOW_WIDTH 600

using namespace std;
using namespace glm;
const int width = 640;
const int height = 480;
vec3** image = new vec3 * [height];
vec3** renderImage = new vec3 * [height * 3];
vector<Shape*> sceneShapes;
vector<Box> sceneBoxes(1000);
unsigned int nthreads = std::thread::hardware_concurrency();

//scene parameters

vec3 lightIntensity = vec3(1.0, 1.0, 1.0);
vec3 specularColour = vec3(0.7, 0.7, 0.7);
vec3 ambient = vec3(0.1, 0.1, 0.1);

vec3 boxLightPos = vec3(2.5, 20, 2);//vec3(-2.5, 40, -2.5);
//vec3 boxLightPos = vec3(1,20,1);
vec3 boxSize = vec3(6, 0.1, 6);

int sample = 25; // always something with sqrt that is int
float sqSample = (int)sqrt(sample);
float divisionx = abs(boxSize.x * 2) / sqSample;
int rayTracingDepth = 2;
int rayTracingSample = 2;
bool antyAliasing = false;
bool animation = false;

void findingPoints(Box container) {
	Box tempBox;
	tempBox.pMin = container.boxShapes[0]->getMin();
	tempBox.pMax = container.boxShapes[0]->getMax();
	for (int i = 1; i < container.boxShapes.size(); i++) {
		vec3 tempMax = container.boxShapes[i]->getMax();
		vec3 tempMin = container.boxShapes[i]->getMin();
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
	container = tempBox;
	//cout << container.pMin.x << " ___ " << container.pMin.y << " ___ " << container.pMin.z << " ___ " <<
		//container.pMax.x << " ___ " << container.pMax.y << " ___ " << container.pMax.z << endl;

}

void createSpheres() {
	/*sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.5, 0.1, 0.5)));
	//sceneShapes.push_back(new BoxReflect(vec3(0, 0, -5.0), vec3(5.0, 5.0, -4.0), vec3(1.0,0,0)));
	sceneShapes.push_back(new Sphere(vec3(0, 0, -20), 4.0f, vec3(1.00, 0.32, 0.36)));
	sceneShapes.push_back(new Sphere(vec3(5, -1, -15), 2.0f, vec3(0.90, 0.76, 0.46)));
	sceneShapes.push_back(new Sphere(vec3(5, 0, -25), 3.0f, vec3(0.65, 0.77, 0.97)));
	sceneShapes.push_back(new Sphere(vec3(-5.5, 0, -15), 3.0f, vec3(0.90, 0.90, 0.90)));
	*/
	
	sceneBoxes[0].boxShapes.push_back(new Sphere(vec3(0, 0, -20), 4.0f, vec3(1.00, 0.32, 0.36)));
	sceneBoxes[0].boxShapes.push_back(new Sphere(vec3(-5.5, 0, -15), 3.0f, vec3(0.90, 0.90, 0.90)));
	sceneBoxes[2].boxShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.5, 0.1, 0.5)));
	//sceneBoxes[3].boxShapes.push_back(new Plane(vec3(10, 0, 0), vec3(1, 0, 0), vec3(0.5, 0.1, 0.5)));
	sceneBoxes[1].boxShapes.push_back(new Sphere(vec3(5, -1, -15), 2.0f, vec3(0.90, 0.76, 0.46)));
	sceneBoxes[1].boxShapes.push_back(new Sphere(vec3(5, 0, -25), 3.0f, vec3(0.65, 0.77, 0.97)));
	
	sceneBoxes.resize(3);
	for (int i = 0; i < sceneBoxes.size(); i++) {
		sceneBoxes[i].findingPoints();
	}
	//one.boxShapes.push_back(&sceneShapes[0]);
	/*for(int i = 0; i < sceneShapes.size(); i++) {
		Box temp = sceneShapes[i]->getBox();
		cout << temp.pMin.x << " ___ " << temp.pMin.y << " ___ " << temp.pMin.z << " ___ " <<
			temp.pMax.x << " ___ " << temp.pMax.y << " ___ " << temp.pMax.z << " ___ " << endl;
		sceneBoxes.push_back(temp);

	}*/


}
void createTriangle() {
	sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.8, 0.8, 0.8)));
	sceneShapes.push_back(new Triangle(vec3(0, 1, -2), vec3(-1.9, -1, -2), vec3(1.6, -0.5, -2),
		vec3(0.0, 0.6, 1.0), vec3(-0.4, -0.4, 1.0), vec3(0.4, -0.4, 1.0), vec3(0.5, 0.5, 0.0)));

}
void saveImage() {
	// Save result to a PPM image
	std::ofstream ofs("./untitled.ppm", std::ios::out | std::ios::binary);
	ofs << "P6\n" << width << " " << height << "\n255\n";
	for (unsigned y = 0; y < height; ++y)
	{
		for (unsigned x = 0; x < width; ++x)
		{
			ofs << (unsigned char)(min((float)1, (float)image[y][x].x) * 255) <<
				(unsigned char)(min((float)1, (float)image[y][x].y) * 255) <<
				(unsigned char)(min((float)1, (float)image[y][x].z) * 255);
		}
	}
	ofs.close();

}



float generalTime = 0.f;
float divisionz = abs(boxSize.z * 2) / sqSample;
void screenImage() {
	/*for (int i = 0; i < width * 3; i++) {
		for (int j = 0; j < height * 3; j++) {
			renderImage[j][i] = image[j / 3][i / 3];
			//cout << "Image: " << j << " : " << i << " = " << j / 3 << " : " << i / 3 << endl;
		}
	}
	for (int i = 0; i < (width - 2) * 3; i++) {
		for (int j = 0; j < (height - 2) * 3; j++) {
			//cout << j << " __ " << j / 3 << endl;
			renderImage[j][i] = (image[j / 3][i / 3] + image[(j / 3) + 1][(i / 3)] + image[(j / 3) + 2][(i / 3)]
				+ image[(j / 3)][(i / 3) + 1] + image[(j / 3) + 1][(i / 3) + 1] + image[(j / 3) + 2][(i / 3) + 1]
				+ image[(j / 3)][(i / 3) + 2] + image[(j / 3) + 1][(i / 3) + 2] + image[(j / 3) + 2][(i / 3) + 2]) / 9.f;
		}
	}*/
	for (int i = 0; i < width * 2; i++) {
		for (int j = 0; j < height * 2; j++) {
			renderImage[j][i] = image[j / 2][i / 2];
			//cout << "Image: " << j << " : " << i << " = " << j / 3 << " : " << i / 3 << endl;
		}
	}
	if (antyAliasing) {
		for (int i = 0; i < (width - 2) * 3; i++) {
			for (int j = 0; j < (height - 2) * 3; j++) {
				//cout << j << " __ " << j / 3 << endl;
				renderImage[j][i] = (image[j / 3][i / 3] + image[(j / 3)][(i / 3)] + image[(j / 3) + 2][(i / 3)]
					+ image[(j / 3)][(i / 3) + 1] + image[(j / 3) + 1][(i / 3) + 1] + image[(j / 3) + 2][(i / 3) + 1]
					+ image[(j / 3)][(i / 3) + 2] + image[(j / 3) + 1][(i / 3) + 2] + image[(j / 3) + 2][(i / 3) + 2]) / 9.f;
			}
		}
		/*for (int i = 0; i < (width - 2) * 2; i++) {
			for (int j = 0; j < (height - 2) * 2; j++) {
				//cout << j << " __ " << j / 3 << endl;
				renderImage[j][i] = (image[j / 2][i / 2] + image[(j / 2) + 1][(i / 2)]
					+ image[(j / 2)][(i / 2) + 1] + image[(j / 2) + 1][(i / 2) + 1]) / 4.f;
			}
		}*/
	}

}
void screenPixels(SDL_Renderer* renderer) {

	//screenImage();
	
	
	for (int i = 0; i < width * 2; i++) {
		for (int j = 0; j < height * 2; j++) {

			SDL_SetRenderDrawColor(renderer, (unsigned char)(min((float)1, (float)renderImage[j][i].x) * 255),
				(unsigned char)(min((float)1, (float)renderImage[j][i].y) * 255),
				(unsigned char)(min((float)1, (float)renderImage[j][i].z) * 255),
				255);

			SDL_RenderDrawPoint(renderer, i, j);
		}
	}
	SDL_RenderPresent(renderer);

	

}
vec3 rayTracing(vec3 origin, vec3 direction, int depth) {
	vec3 tempImage;
	
	
	vec3 reflected;
	HitInfo temp;
	HitInfo* minD = nullptr;
	//vector <tempHit> tempHits;
	vector <HitInfo> tempHits;
	//cout << ray.x << "__" << ray.y << "__" << ray.z << endl;
		
	for (int i = 0; i < sceneBoxes.size(); ++i) {
		if (sceneBoxes[i].intersect(origin, direction)) {
			for (int j = 0; j < sceneBoxes[i].boxShapes.size(); ++j) {

				if (sceneBoxes[i].boxShapes[j]->intersect(origin, direction, temp)) {
					tempHits.push_back(temp);

				}
			}
		}
	}
	//finding closest ray
	if (tempHits.size() > 0) {
		HitInfo proper = tempHits[0];
		for (int g = 0; g < tempHits.size(); g++) {
			if (tempHits[g].t < proper.t) {
				proper = tempHits[g];
			}
		}
		minD = &proper;
	}
	tempHits.clear();
		
	if (minD != nullptr)
	{

		vec3 normal = normalize(minD->normal);
		vec3 fixedPoint = minD->point + normal * (float)1e-4;
		vec3 l = boxLightPos - fixedPoint;
		//vec3 l = lightPos - minD->point;
		l = normalize(l);
		vec3 r = 2.f * dot(l, normal) * normal - l;
		r = normalize(r);
		direction = normalize(direction);
		HitInfo doesntMetter;
		srand((int)time(0));
		vec3 temp = vec3(0.f, 0.f, 0.f);


			
		if (minD->id == 0) { //if surface is plane
			Box minBox;
			//lightning
				
			vector <Box> tempBoxes;
			for (int i = 0; i < sceneBoxes.size(); i++) {
				if (sceneBoxes[i].intersect(fixedPoint, normalize(boxLightPos - fixedPoint))) {
					tempBoxes.push_back(sceneBoxes[i]);
				}
				else if (sceneBoxes[i].intersect(fixedPoint, normalize(vec3(boxLightPos.x - boxSize.x / 2.f, boxLightPos.y, boxLightPos.z + boxSize.z / 2.f) - fixedPoint))) {
					tempBoxes.push_back(sceneBoxes[i]);
				}
				else if (sceneBoxes[i].intersect(fixedPoint, normalize(vec3(boxLightPos.x - boxSize.x / 2.f, boxLightPos.y, boxLightPos.z - boxSize.z / 2.f) - fixedPoint))) {
					tempBoxes.push_back(sceneBoxes[i]);
				}
				else if (sceneBoxes[i].intersect(fixedPoint, normalize(vec3(boxLightPos.x + boxSize.x / 2.f, boxLightPos.y, boxLightPos.z + boxSize.z / 2.f) - fixedPoint))) {
					tempBoxes.push_back(sceneBoxes[i]);
				}
				else if (sceneBoxes[i].intersect(fixedPoint, normalize(vec3(boxLightPos.x + boxSize.x / 2.f, boxLightPos.y, boxLightPos.z - boxSize.z / 2.f) - fixedPoint))) {
					tempBoxes.push_back(sceneBoxes[i]);
				}
					
			}
			for (int xi = 0; xi < int(sqSample); ++xi) {
				for (int y = 0; y < int(sqSample); ++y) {
					float x = (-boxSize.x + divisionx * xi) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((-boxSize.x + divisionx * (xi + 1)) - (-boxSize.x + divisionx * xi))));
					float z = (-boxSize.z + divisionz * y) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / ((-boxSize.z + divisionz * (y + 1)) - (-boxSize.z + divisionz * y))));
					bool isShadow = false;
					vec3 directionVec = normalize(vec3(x, boxLightPos.y, z) - fixedPoint);
					for (int i = 0; i < tempBoxes.size(); i++) {
						for (int j = 0; j < tempBoxes[i].boxShapes.size(); j++) {
							if (tempBoxes[i].boxShapes[j]->intersect(fixedPoint, directionVec, doesntMetter) && doesntMetter.t >= 0.0f) {
								isShadow = true;
								break;

							}
							else {
								if (!isShadow) {
									isShadow = false;
								}

							}
						}
						if (isShadow) {
							break;
						}
							
					}
					if (isShadow) {
						tempImage += (minD->colour * ambient);
					}
					else {
						tempImage += ((minD->colour * ambient)
							+ (minD->colour * lightIntensity * max(0.0f, dot(l, normal)))
							+ (specularColour * lightIntensity * pow(max(0.0f, dot(r, -direction)), 100.0f)));
					}
						
				}
			}
			tempImage /= sample;


		}
		else if (minD->id == 2) {
			tempImage = ((minD->colour * ambient)
				+ (minD->colour * lightIntensity * max(0.0f, dot(l, normal))) 
				+ (specularColour * lightIntensity * pow(max(0.0f, dot(r, -direction)), 100.0f))); 
		}
		else {
			tempImage = ((minD->colour * ambient)
				+ (minD->colour * lightIntensity * max(0.0f, dot(l, normal)))
				+ (specularColour * lightIntensity * pow(max(0.0f, dot(r, -direction)), 100.0f)));

		}
		//reflections

		int couter = 0;
		reflected = direction - 2.f * (dot(direction, normalize(minD->normal))) * normalize(minD->normal);
		reflected = normalize(reflected);
		HitInfo refelction;
		vec3 tempReflect = vec3(0.f, 0.f, 0.f);
		for (int k = 0; k < rayTracingDepth; k++) {
			float x = -0.0001f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.0001f - -0.0001f)));
			float y = -0.0001f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.0001f - -0.0001f)));
			float z = -0.0001f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.0001f - -0.0001f)));
			if (!((depth + 1) > rayTracingDepth)) {
				vec3 check = rayTracing(fixedPoint, normalize(vec3(reflected.x + x, reflected.y + y, reflected.z + z)), depth + 1);
				if (check == vec3(0.0f, 1.0f, 1.0f)) {
					couter++;
					//tempReflect = vec3(0, 0, 0);
					//cout << "trytry" << endl;
					break;
				}
				else {
					tempReflect += specularColour * check;
				}
			}
		}
		tempReflect /= rayTracingDepth /** rayTracingSample*/ - couter;
		tempImage = tempImage + tempReflect;//(tempImage + tempReflect) / 2.f;
			

	}
	else
	{
		tempImage = vec3(0.0f, 1.0f, 1.0f);

	}
	return tempImage;
}

void rayCasting(int w1, int w2, int h1, int h2, SDL_Renderer* renderer)
{
	
	vec3 rayOrigin = vec3(0, 0, 0);
	float aspectRatio = width / height;
	int i = w1;
	
	for (int j = h1; j < h2; j++) {
		


		vec2 pixelNormalized;
		vec2 pixelRemappedx;
		vec2 pixelCamera;
		vec3 cameraSpace;
		vec3 rayDirection;


		pixelNormalized.x = ((i + 0.5) / width);
		pixelNormalized.y = ((j + 0.5) / height);

		pixelRemappedx.x = (2 * pixelNormalized.x - 1) * aspectRatio;
		pixelRemappedx.y = 1 - 2 * pixelNormalized.y;

		pixelCamera.x = pixelRemappedx.x * tan(radians(80.f / 2)); // 0.26794931268;
		pixelCamera.y = pixelRemappedx.y * tan(radians(80.f / 2));// 0.26794931268;

		cameraSpace = vec3(pixelCamera.x, pixelCamera.y, -1);

		rayDirection = normalize(cameraSpace - rayOrigin);

		vec3 temp = rayTracing(rayOrigin, rayDirection, 0);
		renderImage[(j * 2)][(i * 2)] = temp;
		renderImage[(j * 2) + 1][(i * 2)] = temp;
		renderImage[(j * 2)][(i * 2) + 1] = temp;
		renderImage[(j * 2) + 1][(i * 2) + 1] = temp;

	}
}

// Read our .obj file
vector< vec3 > vertices;
vector< vec3 > normals;

void loadOBJ(const char* filePath) {
	bool res = loadOBJ(filePath, vertices, normals);
	
	
	float correctionX = 2;
	float correctionY = -4.5;
	float correctionZ = -15;

	if (filePath == "include/models/teapot_smooth.obj") {
		correctionY = -2.5;
	}
	if (filePath == "include/models/bunny.obj") {
		correctionX = -2;
		correctionY = -4.5;
	}
	if (filePath == "include/models/Town.obj") {
		correctionX = -10;
		correctionY = 3;
		correctionZ = -25;
	}

	


	int c = 0;
	for (int i = 0; i < vertices.size(); i += 3) {
		c = i / 300;
		sceneBoxes[c].boxShapes.push_back(new Triangle(vec3(vertices[i].x + correctionX, vertices[i].y + correctionY, vertices[i].z + correctionZ),
			vec3(vertices[i + 1].x + correctionX, vertices[i + 1].y + correctionY, vertices[i + 1].z + correctionZ),
			vec3(vertices[i + 2].x + correctionX, vertices[i + 2].y + correctionY, vertices[i + 2].z + correctionZ),
			vec3(normals[i].x, normals[i].y, normals[i].z),
			vec3(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z),
			vec3(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z),
			vec3(0.2, 0.5, 0.2)));
		//cout << k << endl;
		/*sceneBoxesAll[c / 10 + c % 10].insideBoxes[c].boxShapes.push_back(new Triangle(vec3(vertices[i].x + correctionX, vertices[i].y + correctionY, vertices[i].z + correctionZ),
			vec3(vertices[i + 1].x + correctionX, vertices[i + 1].y + correctionY, vertices[i + 1].z + correctionZ),
			vec3(vertices[i + 2].x + correctionX, vertices[i + 2].y + correctionY, vertices[i + 2].z + correctionZ),
			vec3(normals[i].x, normals[i].y, normals[i].z),
			vec3(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z),
			vec3(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z),
			vec3(0.5, 0.5, 0.0)));
			*/
			/*sceneShapes.push_back(new Triangle(vec3(vertices[i].x + correctionX, vertices[i].y + correctionY, vertices[i].z + correctionZ),
				vec3(vertices[i + 1].x + correctionX, vertices[i + 1].y + correctionY, vertices[i + 1].z + correctionZ),
				vec3(vertices[i + 2].x + correctionX, vertices[i + 2].y + correctionY, vertices[i + 2].z + correctionZ),
				vec3(normals[i].x, normals[i].y, normals[i].z),
				vec3(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z),
				vec3(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z),
				vec3(0.5, 0.5, 0.0)));*/

	}
	
	sceneBoxes.resize(c + 1);
	//sceneBoxesAll.resize(c / 10 + c % 10 + 1);

}
void sceneCast(SDL_Renderer* renderer) {



	vector<thread> v(thread::hardware_concurrency());
	int divided = width / nthreads;

	int lines = 0;
	while (lines < width) {
		//rayCasting(lines, 0, 0, height, renderer);
		//lines++;
		for (int i = 0; i < v.size(); ++i) {
			v[i] = thread(rayCasting, lines, divided, 0, height, renderer);
			if(lines % 150 == 0 && animation){
				screenPixels(renderer);
			}
			lines++;
		}
		for (unsigned i = 0; i < v.size(); ++i)
		{
			v[i].join();
		}
	}

	screenPixels(renderer);
}

int render() {
	SDL_Event event;
	SDL_Renderer* renderer;
	SDL_Window* window;
	int i;
	enum renderStatus
	{
		inicial,
		up,
		down,
		left,
		right,
		RCTRL
	};
	renderStatus state = inicial;
	SDL_Init(SDL_INIT_VIDEO);
	int screenWidht = width * 3;
	int screenHeight = height * 3;
	SDL_CreateWindowAndRenderer(width * 2, height * 2, 0, &window, &renderer);
	//screenPixels(renderer);
	const clock_t aniamtionTime = clock();
	float clicks = 600000.f;
	while (1) {
		
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_RCTRL] && state != RCTRL)
		{
			state = RCTRL;
			const clock_t begin_time = clock();
			sceneShapes.clear();
			vertices.clear();
			normals.clear();
			sceneBoxes.clear();
			sceneBoxes.resize(1000);
			//sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.8, 0.8, 0.8)));

			loadOBJ("include/models/bunny.obj");

			Box plane;
			plane.boxShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.2, 0.3, 0.3)));
			sceneBoxes.push_back(plane);
			for (int i = 0; i < sceneBoxes.size(); i++) {
				sceneBoxes[i].findingPoints();
			}
			sceneCast(renderer);
			//screenPixels(renderer);
			std::cout << "time to render bunny obj: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
			
		}
		if (currentKeyStates[SDL_SCANCODE_DOWN] && state != down)
		{
			state = down;
			const clock_t begin_time = clock();
			sceneShapes.clear();
			vertices.clear();
			normals.clear();
			sceneBoxes.clear();
			sceneBoxes.resize(1000);
			//sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.8, 0.8, 0.8)));
			
			loadOBJ("include/models/dragon.obj");
		
			Box plane;
			plane.boxShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.2, 0.3, 0.3)));
			sceneBoxes.push_back(plane);
			for (int i = 0; i < sceneBoxes.size(); i++) {
				sceneBoxes[i].findingPoints();
			}
			sceneCast(renderer);
			//screenPixels(renderer);
			std::cout << "time to render dragon obj: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
			
		}
		if (currentKeyStates[SDL_SCANCODE_RIGHT] && state != right)
		{
			state = right;
			const clock_t begin_time = clock();
			sceneShapes.clear();
			vertices.clear();
			normals.clear();
			sceneBoxes.clear();
			sceneBoxes.resize(1000);
			//sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.8, 0.8, 0.8)));
			loadOBJ("include/models/teapot_smooth.obj");
			Box plane;
			plane.boxShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.2, 0.3, 0.3)));
			sceneBoxes.push_back(plane);
			for (int i = 0; i < sceneBoxes.size(); i++) {
				sceneBoxes[i].findingPoints();
			}
			sceneCast(renderer);
			//screenPixels(renderer);
			std::cout << "time to render smooth teapot obj: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		}
		if (currentKeyStates[SDL_SCANCODE_UP] && state != up)
		{
			state = up;
			const clock_t begin_time = clock();

			sceneShapes.clear();
			sceneBoxes.clear();
			sceneBoxes.resize(1000);
			createSpheres();

			sceneCast(renderer);
			//screenPixels(renderer);
			std::cout << "time to render shapes: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;;
		}
		if (currentKeyStates[SDL_SCANCODE_LEFT] && state != left)
		{
			state = left;
			const clock_t begin_time = clock();
			sceneShapes.clear();
			vertices.clear();
			normals.clear();
			sceneBoxes.clear();
			sceneBoxes.resize(1000);
			//sceneShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.8, 0.8, 0.8)));
			loadOBJ("include/models/Town.obj");
			Box plane;
			plane.boxShapes.push_back(new Plane(vec3(0, 5, 0), vec3(0, 1, 0), vec3(0.2, 0.3, 0.3)));
			sceneBoxes.push_back(plane);
			for (int i = 0; i < sceneBoxes.size(); i++) {
				sceneBoxes[i].findingPoints();
			}
			sceneCast(renderer);
			//screenPixels(renderer);
			std::cout << "time to render town obj: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;
		}
		if (currentKeyStates[SDL_SCANCODE_R]) {
			const clock_t begin_time = clock();
			sceneCast(renderer);
			std::cout << "time to rerender scene: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;;
		}

		clicks += float(clock() - aniamtionTime) / CLOCKS_PER_SEC;
		if (currentKeyStates[SDL_SCANCODE_A]) {
			if (clicks > 600000.f) {
				animation = !animation;
				if (animation) {
					cout << "aniamtion is on" << endl;
				}
				else {
					cout << "animation is off" << endl;
				}
				clicks = 0.f;
			}
			//std::cout << "time to rerender scene: " << clicks << endl;
		}
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}
	saveImage();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}

int main(int argc, char* args[])
{
	cout << "Number of threads: " << nthreads << endl;
	cout << "arrow up - spheres \narrow down - dragon obj \narrow left - town obj   \narrow right - teapot smooth obj \nright CTRL - bunny obj \nr - rerender scene \na - enable / disable the animation \nanimation is: ";
	if (animation) {
		cout << "on" << endl;
	}
	else {
		cout << "off" << endl;
	}
	
	for (int i(0); i < height; i++) {
		image[i] = new vec3[width];
	}

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			image[j][i] = vec3(1.0f, 0.0f, 0.0f);
		}
	}
	for (int i(0); i < height * 3; i++) {
		renderImage[i] = new vec3[width * 3];
	}

	for (int i = 0; i < width * 3; i++) {
		for (int j = 0; j < height * 3; j++) {
			renderImage[j][i] = vec3(1.0f, 0.0f, 0.0f);
		}
	}
	render();
	std::cout << "time to draw scene: " << generalTime << endl;

	return 0;
}
