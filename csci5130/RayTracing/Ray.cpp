
/*********************************************
 * Author: Sathish Ramamoorthy               *
 * Class:  CSCI 5130 Computer Graphics       *
 * Assignment: Ray Tracing                   *
 * File: Ray.cpp                             *
 *********************************************/

#include <iostream.h>
#include <math.h>
#include "Ray.h"

void Ray::computeIntersection() {
	intersection.x = origin.x + direction.x * t;
	intersection.y = origin.y + direction.y * t;
	intersection.z = origin.z + direction.z * t;
}

void Ray::makeUnitVector(Point3D *endPoint) {

	direction.x = endPoint->x - origin.x;
	direction.y = endPoint->y - origin.y;
	direction.z = endPoint->z - origin.z; 

	length = sqrt((direction.x*direction.x) + (direction.y*direction.y) + (direction.z*direction.z));

	direction.x /= length;
	direction.y /= length;
	direction.z /= length; 
}

void Point3D::makeUnitVector() {

	double length = sqrt((x*x) + (y*y) + (z*z));

	x /= length;
	y /= length;
	z /= length; 
}

void Point3D::makeUnitVector(Point3D *op) {

	double x1 = x - op->x;
	double y1 = y - op->y;
	double z1 = z - op->z;

	double length = sqrt((x1*x1) + (y1*y1) + (z1*z1));

	x = x1/length;
	y = y1/length;
	z = z1/length; 
}

void Ray::setOrigin(Point3D *o) {
	origin.x = o->x;
	origin.y = o->y;
	origin.z = o->z;
}

void Ray::attenuate() {
	double att;
	att = I.r > 1.0 ? I.r : 1.0;
	att = I.g > 1.0 ? I.g : 1.0;
	att = I.b > 1.0 ? I.b : 1.0;
	I.r /= att;
	I.g /= att;
	I.b /= att;
}

void Object::calculateNormal(Ray *ray) {

	if (objectType == S) {
		normal.x = (ray->intersection.x - center.x) * SrInverse;
		normal.y = (ray->intersection.y - center.y) * SrInverse;
		normal.z = (ray->intersection.z - center.z) * SrInverse;
	} else if (objectType == P) {

	}

}

void Object::calculatePlaneEquation() {
	Point3D *P1 = &vertexCoord[0];
	Point3D *P2 = &vertexCoord[1];
	Point3D *P3 = &vertexCoord[2];

	double v1, v2, v3, w1, w2, w3, A, B, C;
	v1 = P2->x - P1->x;
	v2 = P2->y - P1->y;
	v3 = P2->z - P1->z;
	w1 = P3->x - P1->x;
	w2 = P3->y - P1->y;
	w3 = P3->z - P1->z;

	A = v2*w3 - v3*w2;
	B = v3*w1 - v1*w3;
	C = v1*w2 - v2*w1;

	normal.x = A;
	normal.y = B;
	normal.z = C;
	normal.makeUnitVector();

	D = sqrt(pow(P1->x, 2) + pow(P1->y, 2) + pow(P1->z, 2));

}

void Ray::print() {
	cout << "origin" << endl;
	cout << Ray::origin.x << " " << Ray::origin.y << " " << Ray::origin.z << endl;
	cout << "unit ray" << endl;
	cout << Ray::direction.x << " " << Ray::direction.y << " " << Ray::direction.z << endl;

}

void Lights::print() {
	cout << "Lights, total: " << Lights::numLights << endl;
	cout << "Ambient Intensity " << Lights::Ia.r << " " << Lights::Ia.g << " " << Lights::Ia.b << endl;
	cout << "Ambient Reflectance " << Lights::Ka.r << " " << Lights::Ka.g << " " << Lights::Ka.b << endl;
	for (int i=0; i<Lights::numLights; i++) {
		cout << Lights::lightLocation[i].x << " " << Lights::lightLocation[i].y << " " <<Lights::lightLocation[i].z << " " << endl;
	}
}