#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "Rectangle.h"
#include "Point.h"
#include <iostream>

class Rectangle
{
private:
	Point leftUp, rightDown;

public:
	Rectangle();
	Rectangle(Point a, Point b);
	~Rectangle();

	void setLeftPoint(const Point &newL);

	void setRightPoint(const Point &newR);

	Point getLeftPoint();

	Point getRightPoint();

	bool inside(const Point &b);

	bool checkOverlap(Rectangle b);
	bool greaterOrEqual(const float &a, const float &b);
};

#endif