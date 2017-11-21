#ifndef _OBJECT_H
#define _OBJECT_H

#include <SFML\Graphics.hpp>
#include "Point.h"
#include "Rectangle.h"

class Object
{
public:
	Object();
	Object(const std::vector<int> &frames, const int &width, const int &height);
	~Object();

	void setPosition(const sf::Vector2f &pos);
	bool initSprite(std::string filePath);
	void moveObject(const sf::Vector2f &offset);
	sf::Vector2f getPosition();
	sf::Sprite getSprite();
	void initiateHitbox(Point a, Point b);
	void setTexture(sf::Texture &newT);
	void setFrames(const std::vector<int> &frames);
	void setSpriteWidth(const int &newW);
	void setSpriteHeight(const int &newH);
	bool checkCollision(Object b);
protected:
	sf::Texture mainTexture;
	sf::Sprite mainSprite;
	int spriteWidth, spriteHeight;
	//int numberOfAnimations;
	std::vector<int> numberOfFrames;
	Rectangle hitbox;
};

#endif