#pragma once

void setup(int seed);
float generate(float x, float y);
float calculate(int x, int y, int scale, float maxValue);
int fastFloor(float x);
int mod(int x, int m);
float grad(int hash, float x, float y);