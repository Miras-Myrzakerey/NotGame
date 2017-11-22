#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::Object(const std::vector<int> &frames, const int &width, const int &height)
{
	this->spriteWidth = width;
	this->spriteHeight = height;
	this->numberOfFrames = frames;
}

Object::~Object()
{
}

void Object::setPosition(const sf::Vector2f &pos) {
	mainSprite.setPosition(pos);
	sf::Vector2f cur = mainSprite.getPosition();
	hitbox.setLeftPoint(Point(cur.x, cur.y));
	hitbox.setRightPoint(Point(cur.x + spriteWidth, cur.y + spriteHeight));
}

bool Object::initSprite(std::string filePath) {
	if (!mainTexture.loadFromFile(filePath)) {
		return false;
	} else {
		mainSprite.setTexture(mainTexture);	
		mainSprite.setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight));
		return true;
	}
}

void Object::moveObject(const sf::Vector2f &newPos) {
	setPosition(newPos);
}

sf::Vector2f Object::getPosition() {
	return mainSprite.getPosition();
}

sf::Sprite Object::getSprite() {
	return mainSprite;
}

void Object::initiateHitbox(Point a, Point b)
{
	hitbox.setLeftPoint(a);
	hitbox.setRightPoint(b);
}

void Object::setTexture(sf::Texture &newT)
{
	mainSprite.setTexture(newT);
}

void Object::setFrames(const std::vector<int>& frames)
{
	numberOfFrames = frames;
}

void Object::setSpriteWidth(const int & newW)
{
	spriteWidth = newW;
}

void Object::setSpriteHeight(const int & newH)
{
	spriteHeight = newH;
}

bool Object::checkCollision(Object b)
{
	return hitbox.checkOverlap(b.hitbox);
}

void Object::printHitbox()
{
	hitbox.getLeftPoint().print();
	hitbox.getRightPoint().print();
}
