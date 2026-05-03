
/************************************************
 *                                              *
 * Author: Sathish Ramamoorthy                  *
 * Course: CSCI 5102                            *
 * Project 2: Simulation of Coroutines in C++   *
 * Due Date: 10/23/2000                         *
 *                                              *
 ************************************************/

#include <stdlib.h>
#include <iostream.h>

int x;

void producer();
void consumer();
int  getRand();

int main() {

	producer();

	return 0;

}

void producer() {

	static int c=1;

K:	switch(c) {
		case 1:
			c++;
			x = getRand();
			if (x == 0) return; // Senti
			consumer();
			break;
		case 2:
			c = 1;
			x = getRand();
			if (x == 0) return;
			consumer();
			break;
	}
	
	goto K;

}

void consumer() {

	static int c=1;
	static int A, B, C;

	switch(c) {
		case 1:
			c++;
			A = x;
			break;
		case 2:
			c++;
			B = x;
			break;
		case 3:
			c = 1;
			C = x;
			cout << "A=" << A << ", B=" << B << ", C=" << C << endl;
			break;
	}

}

// produces a random number between 0-100
int getRand() {

	int x;
	x = rand();
	x = x % 100; 
	return x;

}
