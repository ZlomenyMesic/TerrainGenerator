#include <stdlib.h>
#include "simplex.h"

// char = byte
char PERM[512];

// sets up PERM array
void setup(int seed) {
	srand(seed);

	// setup the permutation array
	for (int i = 0; i < 512; i++) {
		PERM[i] = rand() % 256; // % 256 -> MAX = 255, MIN = 0
	}
}

float calculate(int x, int y, int scale, float maxValue) {
	return generate((float)x / scale, (float)y / scale ) * (maxValue / 2) + (maxValue / 2);
}

// DO NOT USE - use calculate ( float(int, int, int, float) )
float generate(float x, float y) {
	const float F2 = 0.366025403f; // F2 = 0.5*(sqrt(3.0)-1.0)
	const float G2 = 0.211324865f; // G2 = (3.0-Math.sqrt(3.0))/6.0

	float n0, n1, n2; // Noise contributions from the three corners

	// Skew the input space to determine which simplex cell we're in
	float s = (x + y) * F2; // Hairy factor for 2D
	float xs = x + s;
	float ys = y + s;
	int i = fastFloor(xs);
	int j = fastFloor(ys);

	float t = (i + j) * G2;
	float X0 = i - t; // Unskew the cell origin back to (x,y) space
	float Y0 = j - t;
	float x0 = x - X0; // The x,y distances from the cell origin
	float y0 = y - Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
	if (x0 > y0) {
		i1 = 1; j1 = 0; // lower triangle, XY order: (0,0)->(1,0)->(1,1)
	} else { 
		i1 = 0; j1 = 1; // upper triangle, YX order: (0,0)->(0,1)->(1,1)
	}

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6

	float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0f + 2.0f * G2;
	     
	// Wrap the integer indices at 256, to avoid indexing perm[] out of bounds
	int ii = Mod(i, 256);
	int jj = Mod(j, 256);

	// Calculate the contribution from the three corners
	float t0 = 0.5f - x0 * x0 - y0 * y0;
	if (t0 < 0.0f) n0 = 0.0f;
	else {
		t0 *= t0;
		n0 = t0 * t0 * grad(PERM[ii + PERM[jj]], x0, y0);
	}

	float t1 = 0.5f - x1 * x1 - y1 * y1;
	if (t1 < 0.0f) n1 = 0.0f;
	else {
		t1 *= t1;
		n1 = t1 * t1 * grad(PERM[ii + i1 + PERM[jj + j1]], x1, y1);
	}

	float t2 = 0.5f - x2 * x2 - y2 * y2;
	if (t2 < 0.0f) n2 = 0.0f;
	else {
		t2 *= t2;
		n2 = t2 * t2 * grad(PERM[ii + 1 + PERM[jj + 1]], x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 40.0f * (n0 + n1 + n2); // TODO: The scale factor is preliminary!
}


int fastFloor(float x) {
	return (x > 0) ? ((int)x) : (((int)x) - 1);
}

int mod(int x, int m) {
	int a = x % m;
	return a < 0 ? a + m : a;
}

float grad(int hash, float x, float y) {
	int h = hash & 7;
	float u = h < 4 ? x : y;  
	float v = h < 4 ? y : x; 
	return ((h & 1) != 0 ? -u : u) + ((h & 2) != 0 ? -2.0f * v : 2.0f * v);
}