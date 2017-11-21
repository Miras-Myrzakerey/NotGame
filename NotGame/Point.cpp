#include <iostream>
#include "Math.h"
#include "stdafx.h"
#include "Rectangle.h"

#define EPS 1e-7

Point::Point() {}
Point::Point(float x2, float y2) {
		x = x2;
		y = y2;
	};

void Point::print() {
	std::cout << x << ' ' << y << "\n";
}

