#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::Player(const std::vector<int>& frames, const int & width, const int & height)
:Object(frames, width, height){
	this->setFrames(frames);
	this->setSpriteWidth(width);
	this->setSpriteHeight(height);
}

Player::~Player()
{
}

int Player::getActualKey(int keyCode) {
	switch (keyCode) {
		case sf::Keyboard::Left: return 0;
		case sf::Keyboard::Right: return 1;
		case sf::Keyboard::Up: return 2;
		case sf::Keyboard::Down: return 3;
	}
}

void Player::handleMovement(int keyCode, int width, int height) {
	int key = getActualKey(keyCode);
	if (walkingClock.getElapsedTime().asMilliseconds() > 75) {
		setTexture(mainTexture);
		sf::Vector2f cur = mainSprite.getPosition() + displacement[key];

		if (cur.x < 0 || cur.x > width || cur.y < 0 || cur.y > height) {
			return;
		}
		moveObject(cur);
		if (key != currentAnimRow) {
			currentAnimRow = key;
			currentAnimColumn = 0;
		}
		else {
			currentAnimColumn = (currentAnimColumn + 1) % walkingAnimationsNum;
		}

		mainSprite.setTextureRect(sf::IntRect(currentAnimColumn * spriteWidth, currentAnimRow * spriteHeight, spriteWidth, spriteHeight));
		walkingClock.restart();
	}
}

bool Player::setLastSprites(std::vector<std::string> files) {
	for (int i = 0; i < 4; ++i) {
		if (!(lastSprite[i].loadFromFile(files[i]))) {			
			return false;
		}
	}

	return true;
}

bool Player::setLastSprite(std::string filePath, int keyCode) {
	int actualKey = getActualKey(keyCode);
	sf::Texture tempTexture;
	if (!lastSprite[actualKey].loadFromFile(filePath)) {
		return false;
	}

	return true;
}

void Player::setSpeed(int newSpeed) {
	speed = newSpeed;
	displacement[0] = sf::Vector2f(-speed, 0); //go left
	displacement[1] = sf::Vector2f(speed, 0);  //go right
	displacement[2] = sf::Vector2f(0, -speed); //go up
	displacement[3] = sf::Vector2f(0, speed);  //go down
}

void Player::stayIdle(int lastKeyPressed)
{	
	setTexture(lastSprite[getActualKey(lastKeyPressed)]);
	currentAnimRow = 0;
	currentAnimColumn = 0;
	mainSprite.setTextureRect(sf::IntRect(0, 0, spriteWidth, spriteHeight));
}

