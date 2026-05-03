
/*********************************************
 * Author: Sathish Ramamoorthy               *
 * Class:  CSCI 5130 Computer Graphics       *
 * Assignment: Ray Tracing                   *
 * File: Ray.h                               *
 *********************************************/

int const SPHERES9 = 0;
int const SPHERES91 = 1;
int const TEST = 2;
int const TRACE = 3;

class Point3D {

	public:
		double x;
		double y;
		double z;
		Point3D (double p1, double p2, double p3) {
			x = p1;
			y = p2;
			z = p3;
		}
		Point3D () {
			x = 0;
			y = 0;
			z = 0;
		}
		void makeUnitVector();
		void makeUnitVector(Point3D *);
};

class Intensity {
	public:
		double r;
		double g;
		double b;
		Intensity() {
			r = g = b = 0;
		}
		void clearValues() {
			r = g = b = 0;
		};
};

class Color255 {
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
};

class Coeffs {
	public:
		double r;
		double g;
		double b;
};

class Ray {

	public:
		Point3D origin;
		Point3D direction;
		Point3D intersection;
		Intensity I;
		double length;
		double t;
		int traceDepth;
		void computeIntersection();
		void print();
		void makeUnitVector(Point3D *);
		void attenuate();
		void setOrigin(Point3D *);

};

class Lights {
	public:
		Coeffs Ia;
		Coeffs Ka;
		int numLights;
		Point3D *lightLocation;
		Lights() {
			lightLocation = NULL;
		}
		void print();
};

class Surface {
	public:
		static int numSurfaceTypes;
		Coeffs Ka;
		Coeffs Kd;
		Coeffs Ks;
		int phongExp;
};

enum objType {S, P};

class Object {
	public:

		objType objectType;
		int surfaceID;
	
		// data members for sphere
		Point3D center;
		double radius;
		double SrSquare;
		double SrInverse;

		// data members for planes
		int numOfVertices;
		Point3D *vertexCoord;
		double D;
		
		Object() {
			vertexCoord = NULL;
		}

		// to calculate the normal at intersecting point
		Point3D normal;
		void calculateNormal(Ray *);
		void calculatePlaneEquation();
};
