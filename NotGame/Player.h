#ifndef _PLAYER_H
#define _PLAYER_H

#include "Object.h"
#include <SFML\Graphics.hpp>

class Player :
	public Object
{
public:
	Player();
	Player(const std::vector<int> &frames, const int &width, const int &height);
	~Player();
	
	int getActualKey(int keyCode);
	void handleMovement(int keyCode, int width, int height);
	bool setLastSprites(std::vector<std::string> files);
	bool setLastSprite(std::string filePath, int keyCode);
	void setSpeed(int speed);
	void stayIdle(int lastKeyPressed);
private:
	int speed;
	//order: left, right, up, down
	sf::Vector2f displacement[4];
	sf::Texture lastSprite[4];
	sf::Clock walkingClock;
	int currentAnimRow, currentAnimColumn;
	int walkingAnimationsNum = 4;
};

#endif