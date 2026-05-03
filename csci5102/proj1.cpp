
/*********************************************
 * Author: Sathish Ramamoorthy               *
 * Class:  CSCI1502                          *
 * Project1: Program fuction pointers        *
 * Due Date: October 2, 2000                 *
 *********************************************/

#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>

double F1(double, double, double);
double G1(double, double, double);
double H1(double, double, double);

double F2(double, double, double);
double G2(double, double, double);
double H2(double, double, double);

typedef double FGH(double, double, double);

void callFGH(FGH *, FGH *, FGH *, double, double, double);
double a11, a12, a13, a21, a22, a23, a31, a32, a33, b1, b2, b3;
void readMatrix();

int main() {

	double x, y, z;
	readMatrix();

//	x=1.0, y=2.0, z=3.0;
//	callFGH(&F1, &G1, &H1, x, y, z);

	x=0.0, y=1.0, z=2.0;
	callFGH(&F2, &G2, &H2, x, y, z);

	return 0;
}

void callFGH(FGH *F, FGH *G, FGH *H, double x, double y, double z) {

	int i;
	for (i=0; i<100; i++) {
		cout << "(iteration " << i << ")--> x=" << x <<
				" y=" << y << " z=" << z << ";" << endl;
		x = F(x, y, z);
		y = G(x, y, z);
		z = H(x, y, z);
	}

	cout << "(iteration " << i << ")--> x=" << x <<
			" y=" << y << " z=" << z << ";" << endl;

}

double F1(double x, double y, double z) {
	return ((7-2*y*y)/5+x)/2;
}

double G1(double x, double y, double z) {
	return (((10-(2*x+3)*x*x)/3)+2*z-x)/3;
}

double H1(double x, double y, double z) {
	return ((x+y)/2);
}

double F2(double x, double y, double z) {
	return ((b1 - a12*y - a13*z))/a11;
}

double G2(double x, double y, double z) {
//	return ((b1 - a11*x - a13*z))/a12;
	return ((b2 - a21*x - a23*z))/a22;
}

double H2(double x, double y, double z) {
//	return ((b1 - a11*x - a12*y))/a13;
	return ((b3 - a31*x - a32*y))/a33;
}

void readMatrix() {

	ifstream inFile("Matrix.dat", ios::in);
	inFile >> a11 >> a12 >> a13 >> b1;
	inFile >> a21 >> a22 >> a23 >> b2;
	inFile >> a31 >> a32 >> a33 >> b3;

	cout << setw(6) << a11 << " " << setw(6) << a12 << " " 
		 << setw(6) << a13 << " " << setw(6) << b1  << endl;
	cout << setw(6) << a21 << " " << setw(6) << a22 << " "
		 << setw(6) << a23 << " " << setw(6) << b2  << endl;
	cout << setw(6) << a31 << " " << setw(6) << a32 << " " 
		 << setw(6) << a33 << " " << setw(6) << b3  << endl;

}