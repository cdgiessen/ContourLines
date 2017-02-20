#pragma once
#include <gl/freeglut.h>
#include <string>
#include <iostream>
#include <sstream>
#include <array>
#include <fstream>
#include <vector>

#include "ReadGridFloat.h"

#ifndef CONTOUR_LINES_H
#define CONTOUR_LINES_H

struct point {
	float x;
	float y;
};

struct lineSeg {
	struct point a;
	struct point b;
};

void DrawLine(struct point a, struct point b, std::array<float, 3> color);
int getContourCase(bool TL, bool TR, bool BL, bool BR);
void getContourLine(int loopUpCase, int x, int y, float iso, std::vector<lineSeg> *lines);
float lerp(float v1, float v2, float iso, int a1, int a2);
void init();
void display(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void centerOnScreen();
bool fileExists(const std::string& filename);

#endif // !CONTOUR_LINES_H
