
/*********************************************
 * Author: Sathish Ramamoorthy               *
 * Class:  CSCI 5130 Computer Graphics       *
 * Assignment: Ray Tracing                   *
 * File: RayTracer.cpp                       *
 *********************************************/

#include <iostream.h>
#include <math.h>
#include <fstream.h>
#include <string.h>
#include <stdlib.h>
#include <gl/glut.h> 
#include <gl/gl.h>
#include "Ray.h"

#define wWidth	450
#define wHeight 450

void display();
void init();
void shootRay(Ray *);
void calculateAndShootReflectedRay(Ray *, Object *);
Object *findIntersection(Ray *, int, Object *);
void calculateLocalLighting(Object *, Ray *);
void printSurfaces();
void printObjects();
void traceRay();
void createMenus();
void freeMem();
void loadParam();
void viewTransform();
bool nearPlaneClipped(Ray *);
void matrixMult(Point3D *, double *);
void reshape(int, int);
void processMenuEvents(int);
GLubyte image[wHeight][wWidth][3];
bool loadData(char *);
Lights lights;
Surface *surface = NULL;
Object  *object  = NULL;
int numOfSurfaceTypes = 0;
int numOfObjects = 0;

Point3D vrp;
Point3D lookAtDirection;
Point3D upDirection;

double imageWidth;
double imageHeight;

double scaleI = 1.0;	
int depthOfTrace;
double projectionPlane;
const int CLOSEST_OBJECT = 1;
const int SHADOW_CALCULATION = 2;
Point3D R, N, V, L, H;
double NdotV, LdotN, NdotH;
Ray shadowRay;
Intensity lightI;
Intensity localI;
double wloc[4];		// xleft, xright, xtop, xdown
char *fileName = "test.dat";
double vd, vo, tempt;
bool usePlane;

void main(int argc, char **argv) {

//	lights.print();
//	printSurfaces();
//	printObjects();
	traceRay();
	init();
	glutMainLoop();
}

void traceRay() {

	if (!loadData(fileName)) {
		cout << "Unable to load data" << endl;
		return;
	}

	loadParam();
	viewTransform();

	double dx = abs(wloc[0] - wloc[1])/(double)wWidth;
	double dy = abs(wloc[2] - wloc[3])/(double)wHeight;

	double dxStart = wloc[0] + 0.5 * dx;
	double dyStart = wloc[1] - 0.5 * dy;
	double dxNext = dxStart, dyNext = dyStart;

	Point3D pixel(0, 0, -1);
	Ray ray;

	for (int i=0; i<wHeight; i++) {
		for (int j=0; j<wWidth; j++) {
			pixel.x = dxNext;
			pixel.y = dyNext;
	
			// form ray
			ray.I.clearValues();

			ray.setOrigin(&vrp);
			ray.traceDepth = depthOfTrace;

			ray.makeUnitVector(&pixel);
			// end form ray

			shootRay(&ray);

			//write the ray rgb to the pixel array
			ray.attenuate();
			image[i][j][0] = ray.I.r * 255;
			image[i][j][1] = ray.I.g * 255;
			image[i][j][2] = ray.I.b * 255;
//			cout << ray.I.r << " " << ray.I.g << " " << ray.I.b << endl;

			dxNext += dx;
		}
		dyNext -= dy;
		dxNext = dxStart;
	}
}	

void shootRay(Ray *ray) {
	ray->t = 9999999;
	Object *obj = findIntersection(ray, CLOSEST_OBJECT, NULL);
	if (obj != NULL && !nearPlaneClipped(ray)) {
		obj->calculateNormal(ray);

		N.x = obj->normal.x;
		N.y = obj->normal.y;
		N.z = obj->normal.z;

		V.x = ray->origin.x;
		V.y = ray->origin.y;
		V.z = ray->origin.z;
		V.makeUnitVector(&ray->intersection);

		calculateLocalLighting(obj, ray);
		ray->traceDepth--;
		if (ray->traceDepth > 0) {
			calculateAndShootReflectedRay(ray, obj);
		}
	}
}

void calculateAndShootReflectedRay(Ray *ray, Object *obj) {

	NdotV = N.x*V.x + N.y*V.y + N.z*V.z;

	R.x = 2 * N.x * NdotV;
	R.y = 2 * N.y * NdotV;
	R.z = 2 * N.z * NdotV;

	R.x -= V.x;
	R.y -= V.y;
	R.z -= V.z;

	ray->direction.x = R.x;
	ray->direction.y = R.y;
	ray->direction.z = R.z;

	ray->setOrigin(&ray->intersection);

	shootRay(ray);
}

void calculateLocalLighting(Object *obj, Ray *ray) {

	localI.clearValues();
	// Ambient lighting
		if (ray->traceDepth == depthOfTrace)
		{
			localI.r = surface[obj->surfaceID].Ka.r * lights.Ia.r;
			localI.g = surface[obj->surfaceID].Ka.g * lights.Ia.g;
			localI.b = surface[obj->surfaceID].Ka.b * lights.Ia.b;
		}


	for (int i=0; i<lights.numLights; i++) {

		shadowRay.t = 9999999;
		shadowRay.setOrigin(&ray->intersection);
		shadowRay.makeUnitVector(&lights.lightLocation[i]);

		L.x = shadowRay.direction.x;
		L.y = shadowRay.direction.y;
		L.z = shadowRay.direction.z;

		LdotN = L.x * N.x + L.y * N.y + L.z * N.z;
		if (LdotN < 0)
			continue;

		if (findIntersection(&shadowRay, SHADOW_CALCULATION, obj) == NULL) {
//			cout << " Intersection found\n";

			H.x = (L.x + V.x)/2;
			H.y = (L.x + V.y)/2;
			H.z = (L.x + V.z)/2;
			H.makeUnitVector();

			NdotH = pow((N.x * H.x + N.y * H.y + N.z * H.z), surface[obj->surfaceID].phongExp);

//			if (ray->traceDepth != depthOfTrace)
			{
				localI.r += lightI.r * (surface[obj->surfaceID].Kd.r * LdotN + surface[obj->surfaceID].Ks.r * NdotH);	
				localI.g += lightI.g * (surface[obj->surfaceID].Kd.g * LdotN + surface[obj->surfaceID].Ks.g * NdotH);	
				localI.b += lightI.b * (surface[obj->surfaceID].Kd.b * LdotN + surface[obj->surfaceID].Ks.b * NdotH);
			}

		} else {
//			cout << "No Intersection found\n";
		}
	}
//	if (ray->traceDepth == depthOfTrace)
//		scaleI = 1.0;
//	else
//		scaleI = 0.3;
	ray->I.r += localI.r * scaleI;
	ray->I.g += localI.g * scaleI;
	ray->I.b += localI.b * scaleI;
}

Object * findIntersection(Ray *ray, int type, Object *obj) {
	
	Object *closestIntersectingObject = NULL;
	Object *tempObject;
	double raytempt, saverayt;

	for (int i=0; i<numOfObjects; i++) {
		tempObject = NULL;
		if (type == SHADOW_CALCULATION && obj == &object[i])
			continue;
		if (object[i].objectType == S) {			// sphere
			Object sphere = object[i];
			double B, C;  // A=1
			double Xc = sphere.center.x, Yc = sphere.center.y, Zc = sphere.center.z;
			double Xd = ray->direction.x, Yd = ray->direction.y, Zd = ray->direction.z;
			double Xo = ray->origin.x, Yo = ray->origin.y, Zo = ray->origin.z;
			double SrSq = sphere.SrSquare;

			B = 2 * (Xd * (Xo - Xc) + Yd * (Yo - Yc) + Zd * (Zo - Zc));
			C = pow((Xo-Xc),2) + pow((Yo-Yc),2) + pow((Zo-Zc),2) - SrSq;

			double discriminant = pow(B,2) - (4*C);
			if (discriminant > 0) {
				double t0 = (-B - sqrt(discriminant))/2;
				if (t0 > 0) {
					if (t0 < ray->t) {
						raytempt = t0;
						tempObject = &object[i];
					}
				} else {
					double t1 = (-B + sqrt(discriminant))/2;
					if (t1 > 0.00001 && t1 < ray->t) {
						raytempt = t1;
						tempObject = &object[i];
					}
				}

				//cout << ray->t << endl;
				if (type == SHADOW_CALCULATION && tempObject != NULL && raytempt > 0.00001)
					return &object[i];

			}
		} else if (object[i].objectType == P && usePlane) {	// plane
			vd = object[i].normal.x*ray->direction.x +
				 object[i].normal.y*ray->direction.y +
				 object[i].normal.z*ray->direction.z;
			if (vd < 0) {
				vo = - (object[i].normal.x*ray->origin.x +
						object[i].normal.y*ray->origin.y +
						object[i].normal.z*ray->origin.z);
				tempt = vo/vd;
				if (tempt > 0.00001 && tempt < ray->t) {
					raytempt = tempt;
					tempObject = &object[i];
				}
			}
		}
		if (tempObject != NULL && raytempt < ray->t) {
			saverayt = ray->t;
			ray->t = raytempt;
			ray->computeIntersection();
			if (nearPlaneClipped(ray)) {
				ray->t = saverayt;
			} else {
				closestIntersectingObject = tempObject;
			}
		}
	}

	if (closestIntersectingObject != NULL)
		ray->computeIntersection();

	return closestIntersectingObject;
}

void viewTransform() {

	Point3D *n = &lookAtDirection;
	n->makeUnitVector(&vrp);

	Point3D *vp = &upDirection;
	vp->makeUnitVector();

	Point3D *u = new Point3D;
	u->x = (n->y*vp->z) - (n->z*vp->y);
	u->y = (n->z*vp->x) - (n->x*vp->z);
	u->z = (n->x*vp->y) - (n->y*vp->x);

	Point3D *v = new Point3D;
	v->x = (u->y*n->z) - (u->z*n->y);
	v->y = (u->z*n->x) - (u->x*n->z);
	v->z = (u->x*n->y) - (u->y*n->x);

	double a[16];
	a[0]=u->x; a[1]=u->y; a[2]=u->z; a[3]=0;
	a[4]=v->x; a[5]=v->y; a[6]=v->z; a[7]=0;
	a[8]=n->x; a[9]=n->y; a[10]=n->z; a[11]=0;
	a[12]=0; a[13]=0; a[14]=0; a[15]=1;

	matrixMult(&vrp, a);
	for (int i=0; i<lights.numLights; i++) {
		matrixMult(&lights.lightLocation[i], a);
	}
	for (i=0; i<numOfObjects; i++) {
		if (object[i].objectType == S) {
			matrixMult(&object[i].center, a);
		} else if (object[i].objectType == P) {
			for (int j=0; j<object[i].numOfVertices; j++) {
				matrixMult(&object[i].vertexCoord[j], a);
			}
			matrixMult(&object[i].normal, a);
		}
	}
}

void matrixMult(Point3D *p, double *m) {

	p->x = m[0]*p->x + m[4]*p->y + m[8]*p->z + m[12]*1;	//|a0  a4  a8   a12| | x |
														//|		           | |   |
	p->y = m[1]*p->x + m[5]*p->y + m[9]*p->z + m[13]*1;	//|a1  a5  a9   a13| | y |
														//|		           | |   |
	p->z = m[2]*p->x + m[6]*p->y + m[10]*p->z + m[14]*1;//|a2  a6  a10  a14| | z |
														//|		           | |   |
														//|a3  a7  a11  a15| | 1 |

}

bool nearPlaneClipped(Ray *ray) {

	if (ray->traceDepth == depthOfTrace && projectionPlane >
	((ray->origin.x - ray->intersection.x) * (ray->origin.x - lookAtDirection.x) +
	 (ray->origin.y - ray->intersection.y) * (ray->origin.y - lookAtDirection.y) +
	 (ray->origin.z - ray->intersection.z) * (ray->origin.z - lookAtDirection.z) ))
		return true;
	else
		return false;
}

void init() {

	glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(200,200);
	glutInitWindowSize(wWidth,wHeight);
	glutCreateWindow("Computer Graphics: Ray Tracing");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	createMenus();

}

void createMenus() {

	int menu,loadSubMenu;

	loadSubMenu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("spheres.9.dat",SPHERES9);
	glutAddMenuEntry("spheres.91.dat",SPHERES91);
	glutAddMenuEntry("test.dat",TEST);

	menu = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Trace",TRACE);
	glutAddSubMenu("Load file",loadSubMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void loadParam() {

	ifstream inFile("input.txt", ios::in);
	if (!inFile)
		return;

	char inputLine[100];
	char *tok;
	int i;

	while (!inFile.eof()) {
		inFile.getline(inputLine, 100);
		if (strlen(inputLine) == 0)
			continue;
		tok = strtok(inputLine," ");
		if (tok == NULL)
			continue;
		i = 1;
		if (strcmp(tok, "vrp")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					vrp.x = atof(tok);
				else if (i==2)
					vrp.y = atof(tok);
				else if (i==3)
					vrp.z = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "look_at_direction")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					lookAtDirection.x = atof(tok);
				else if (i==2)
					lookAtDirection.y = atof(tok);
				else if (i==3)
					lookAtDirection.z = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "up_direction")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					upDirection.x = atof(tok);
				else if (i==2)
					upDirection.y = atof(tok);
				else if (i==3)
					upDirection.z = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "image_resolution")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					imageWidth = atof(tok);
				else if (i==2)
					imageHeight = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "light_intensity")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					lightI.r = atof(tok);
				else if (i==2)
					lightI.g = atof(tok);
				else if (i==3)
					lightI.b = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "trace_depth")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					depthOfTrace = atoi(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "projection_plane")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1)
					projectionPlane = atoi(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "wloc")==0) {
			tok = strtok (NULL, " ");
			cout << tok << " ";
			while (tok != NULL) {
				if (i==1)
					wloc[0] = atof(tok);
				else if (i==2)
					wloc[1] = atof(tok);
				else if (i==3)
					wloc[2] = atof(tok);
				else if (i==4)
					wloc[3] = atof(tok);
				i++;
				tok = strtok (NULL, " ");
			}
		} else if (strcmp(tok, "use_plane")==0) {
			tok = strtok (NULL, " ");
			while (tok != NULL) {
				if (i==1) {
					if (strcmp(tok, "yes")==0)
						usePlane = true;
					else
						usePlane = false;
				}
				i++;
				tok = strtok (NULL, " ");
			}
		}
	}
}

void processMenuEvents(int option) {
	switch (option) {
		case SPHERES9 : 
			freeMem();
			fileName = "spheres.9.dat";
			traceRay();
			glutPostRedisplay();
			break;
		case SPHERES91 :
			freeMem();
			fileName = "spheres.91.dat";
			traceRay();
			glutPostRedisplay();
			break;
		case TEST : 
			freeMem();
			fileName = "test.dat";
			traceRay();
			glutPostRedisplay();
			break;
		case TRACE : 
			traceRay();
			glutPostRedisplay();
			break;
	}
}

void freeMem() {
	delete [] surface;
	delete [] object;
	delete [] lights.lightLocation;
}

void reshape(int w, int h) {

}

void display() {

    glPushAttrib(GL_ALL_ATTRIB_BITS); 
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glShadeModel(GL_FLAT); 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluOrtho2D(0.0, 1.0, 0.0, 1.0); 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    glClear(GL_COLOR_BUFFER_BIT); 

//  glRasterPos2f (offset[0], offset[1]); 
//  glPixelZoom (zoom[0], zoom[1]); 
    glDrawPixels(wWidth, wHeight, GL_RGB, GL_UNSIGNED_BYTE, image); 

}


bool loadData(char *fileName) {
	
	ifstream inFile(fileName, ios::in);
	if (!inFile)
		return false;

	char inputLine[100];
	enum flag {LIGHTS, SURFACES, OBJECTS};
	flag processFlag = LIGHTS;
	enum flag1 {SPHERE, PLANE};
	flag1 processObject;
	char *tok;
	int i, j , k, m;
	i = j = k = 0, m = 0;
	bool surfacesAllocated = false;
	bool firstLine = false;

	while (!inFile.eof()) {
		inFile.getline(inputLine, 100);

		if (strlen(inputLine) == 0)
			continue;

		if (processFlag == LIGHTS) {
			i = 0;
			tok = strtok(inputLine," ");
			while (tok != NULL) {
				if (k==0) {
					if (i==0)
						lights.Ia.r = atof(tok);
					else if (i==1)
						lights.Ia.g = atof(tok);
					else if (i==2)
						lights.Ia.b = atof(tok);
				} else if (k==1) {
					if (i==0)
						lights.Ka.r = atof(tok);
					else if (i==1)
						lights.Ka.g = atof(tok);
					else if (i==2)
						lights.Ka.b = atof(tok);
				} else if (k==2) {
					lights.numLights = atoi(tok);
					lights.lightLocation = (Point3D*)malloc(sizeof(Point3D) * lights.numLights);
				} else {
					if (i==0)
						lights.lightLocation[j].x = atof(tok);
					else if (i==1)
						lights.lightLocation[j].y = atof(tok);
					else if (i==2) {
						lights.lightLocation[j].z = atof(tok);
						j++;
						if (j == lights.numLights) {
							processFlag = SURFACES;
							j = 0; k = -1;
						}
					}
				}
				i++;
				tok = strtok (NULL, " ");
			}
			k++;
		} else if (processFlag == SURFACES) {
			i = 0;
			tok = strtok(inputLine," ");
			while (tok != NULL) {
				if (k==0) {
					if (!surfacesAllocated) {
						surface = (Surface*)malloc(sizeof(Surface) * atoi(tok));
						numOfSurfaceTypes = atoi(tok);
						surfacesAllocated = true;
					}
				} else if (k==1) {
					if (i==0)
						surface[j].Ka.r = atof(tok);
					else if (i==1) 
						surface[j].Ka.g = atof(tok);
					else if (i==2)
						surface[j].Ka.b = atof(tok);
				} else if (k==2) {
					if (i==0)
						surface[j].Kd.r = atof(tok);
					else if (i==1) 
						surface[j].Kd.g = atof(tok);
					else if (i==2) 
						surface[j].Kd.b = atof(tok);
				} else if (k==3) {
					if (i==0)
						surface[j].Ks.r = atof(tok);
					else if (i==1) 
						surface[j].Ks.g = atof(tok);
					else if (i==2) 
						surface[j].Ks.b = atof(tok);
				} else if (k==4){
					surface[j].phongExp = atof(tok);
					j++;
					k = 0;
					if (j == numOfSurfaceTypes) {
						processFlag = OBJECTS;
						j = 0; k = -1;
					}
				}
				i++;
				tok = strtok (NULL, " ");
			}
			k++;
		} else if (processFlag = OBJECTS) {
			i = 0;
			tok = strtok(inputLine," ");
			while (tok != NULL) {
				if (k==0) {
					object = (Object*)malloc(sizeof(Object) * atoi(tok));
					numOfObjects = atoi(tok);
				} else {
					if (strcmp(tok, "S")==0) {
						processObject = SPHERE;
					} else if (strcmp(tok, "P")==0) {
						processObject = PLANE;
						firstLine = true;
						m = 0;
					} else {
						if (processObject == SPHERE) {
							if (i == 1) {
								object[j].center.x = atof(tok);
								object[j].objectType = S;
							} else if (i == 2) {
								object[j].center.y = atof(tok);
							} else if (i == 3) {
								object[j].center.z = atof(tok);
							} else if (i == 4) {
								object[j].radius = atof(tok);
								object[j].SrSquare = pow(object[j].radius,2);
								object[j].SrInverse = 1 / object[j].radius;
							} else if (i == 5) {
								object[j].surfaceID = atoi(tok);
								j++;
							}
						} else if (processObject == PLANE) {
							if (firstLine) {
								if (i == 1) {
									object[j].numOfVertices = atoi(tok);
									object[j].vertexCoord = (Point3D*)malloc(sizeof(Point3D) * object[j].numOfVertices);
									object[j].objectType = P;
								} else if (i == 2) {
									object[j].surfaceID = atoi(tok);
									firstLine = false;
								}
							} else {
								if (i == 0) {
									object[j].vertexCoord[m].x = atof(tok);
								} else if (i == 1) {
									object[j].vertexCoord[m].y = atof(tok);
								} else if (i == 2) {
									object[j].vertexCoord[m].z = atof(tok);
									m++;
								}
								if (m == object[j].numOfVertices) {
									j++;
								}
							}
						}
					}
				}
				i++;
				tok = strtok (NULL, " ");
			}
			k++;

		}

	}

	for (int p=0; p<numOfObjects; p++) {
		if (object[p].objectType == P)
			object[p].calculatePlaneEquation();
	}

	return true;
}

void printSurfaces() {
	cout << "Surfaces " << numOfSurfaceTypes << endl;
	for (int i=0; i<numOfSurfaceTypes; i++) {
		cout << surface[i].Ka.r << " " << surface[i].Ka.g << " " << surface[i].Ka.b << " " << endl;
		cout << surface[i].Kd.r << " " << surface[i].Kd.g << " " << surface[i].Kd.b << " " << endl;
		cout << surface[i].Ks.r << " " << surface[i].Ks.g << " " << surface[i].Ks.b << " " << endl;
		cout << "phong " << surface[i].phongExp << endl;
	}
}

void printObjects() {
	cout << "Number of objects " << numOfObjects << endl;
	for (int i=0; i<numOfObjects; i++) {
		if (object[i].objectType == S) {
			cout << "S ";
			cout << object[i].center.x << " " << object[i].center.y << " " <<
					object[i].center.z << " " << object[i].radius << " " <<
					object[i].surfaceID << endl;
		} else if (object[i].objectType == P) {
			cout << "P " << object[i].numOfVertices << " " << object[i].surfaceID << endl;
			for (int j=0; j<object[i].numOfVertices; j++) {
				cout << object[i].vertexCoord[j].x << " " <<
						object[i].vertexCoord[j].y << " " <<
						object[i].vertexCoord[j].z << endl;
			}
		}
	}
}