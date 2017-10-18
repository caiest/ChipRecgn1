#pragma once
#include "math.h"
#include <vector>
#include <algorithm>

#define PI 3.141592653589793238462643383279
#define Er (1E-10)
#define Ir 0.9999999999999

using namespace std;


int SeedFill(unsigned char *indata,int cx, int cy, int startX, int startY, unsigned char fillData, unsigned char *fillImg,int &x1,int &x2,int &y1,int &y2);
