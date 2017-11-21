#include "stdafx.h"
#include "Rectangle.h"

#define EPS 1e-7

Rectangle::Rectangle() {}
Rectangle::Rectangle(Point a, Point b) : leftUp(a), rightDown(b) {

}

Rectangle::~Rectangle()
{
}

void Rectangle::setLeftPoint(const Point &newL) {
	leftUp = newL;
}

void Rectangle::setRightPoint(const Point &newR) {
	rightDown = newR;
}

Point Rectangle::getLeftPoint() {
	return leftUp;
}

Point Rectangle::getRightPoint() {
	return rightDown;
}

bool Rectangle::inside(const Point &b) {
	if (greaterOrEqual(b.x, leftUp.x)
		&& greaterOrEqual(rightDown.x, b.x)
		&& greaterOrEqual(b.y, leftUp.y)
		&& greaterOrEqual(rightDown.y, b.y)) {
		return true;
	}

	return false;
}


bool Rectangle::checkOverlap(Rectangle b) {
	Point p[4];
	p[0] = this->leftUp;
	p[1] = this->rightDown;
	p[2] = Point(p[0].x, p[1].y);
	p[3] = Point(p[1].x, p[0].y);
	for (int i = 0; i < 4; ++i) {
		if (b.inside(p[i])) {
			return true;
		}
	}

	p[0] = b.leftUp;
	p[1] = b.rightDown;
	p[2] = Point(p[0].x, p[1].y);
	p[3] = Point(p[1].x, p[0].y);
	for (int i = 0; i < 4; ++i) {
		if (this->inside(p[i])) {
			return true;
		}
	}

	return false;
}

bool Rectangle::greaterOrEqual(const float & a, const float & b)
{
	if (fabs(a - b) < EPS)
		return true;
	return a - b > EPS;
}