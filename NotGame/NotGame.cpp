#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SPRITE_SPEED 3
#define INV_SIZE 10
#define PLAYERX 75
#define PLAYERY 110
#define CHESTX 70
#define CHESTY 70

struct Point {
	float x, y;

public:
	Point() {}
	Point(float x2, float y2) {
		x = x2;
		y = y2;
	};

	void print() {
		std::cout << x << ' ' << y << "\n";
	}
};

struct Rectangle {
private:
	Point leftUp, rightDown;

public:
	Rectangle() {}
	Rectangle(Point a, Point b) : leftUp(a), rightDown(b) {

	}

	void setLeftPoint(const Point &newL) {
		leftUp = newL;
	}

	void setRightPoint(const Point &newR) {
		rightDown = newR;
	}

	Point getLeftPoint() {
		return leftUp;
	}

	Point getRightPoint() {
		return rightDown;
	}

	bool inside(const Point &b) {
		if (b.x >= leftUp.x && b.x <= rightDown.x && b.y >= leftUp.y && b.y <= rightDown.y) {
			return true;
		}

		return false;
	}
}player, chestR;

float min(float a, float b) {
	return a < b ? a : b;
}

float max(float a, float b) {
	return a > b ? a : b;
}

int keys[4] = { sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Down, sf::Keyboard::Up };
int displacement[4][2] = { {-SPRITE_SPEED, 0}, {SPRITE_SPEED, 0}, {0, SPRITE_SPEED}, {0, -SPRITE_SPEED} };
bool pressed[4];
float desktop_width, desktop_height;
sf::Sprite inventory[INV_SIZE], chest, player_sprite;
sf::Sprite dialog;

template <typename T>
inline void swap(T &a, T &b) {
	T c;
	c = a;
	a = b;
	b = c;
}

std::string toString(int x) {
	std::string result = "";
	do {
		result += (char)(x % 10 + 48);
		x /= 10;
	} while (x > 0);

	for (int i = 0; i < (int)result.size(); ++i) {
		swap(result[i], result[result.size() - i - 1]);
	}

	return result;
}

bool checkOverlap(Rectangle a, Rectangle b) {
	Point p[4];
	p[0] = a.getLeftPoint();
	p[1] = a.getRightPoint();
	p[2] = Point(p[0].x, p[1].y);
	p[3] = Point(p[1].x, p[0].y);
	for (int i = 0; i < 4; ++i) {
		if (b.inside(p[i])) {
			p[i].print();
			b.getLeftPoint().print();
			b.getRightPoint().print();
			std::cout << "\n";
			return true;
		}
	}

	return false;
}

void initPlayer() {
	float x = player_sprite.getPosition().x;
	float y = player_sprite.getPosition().y;
	player = Rectangle(Point(x, y), Point(x + PLAYERX, y + PLAYERY));
}

void initChest() {
	float x = chest.getPosition().x;
	float y = chest.getPosition().y;
	chestR = Rectangle(Point(x, y), Point(x + CHESTX, y + CHESTY));
}

int main()
{
	srand(time(NULL));
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	desktop_height = desktop.height;
	desktop_width = desktop.width;
	sf::RenderWindow window(desktop, "SFML works!");
	sf::Texture texture, background_texture, chest_texture;
	sf::Vector2f targetSize(desktop.width, desktop.height);
	sf::Texture walk_textures[4][4], inventory_texture, dialog_texture;

	if (!texture.loadFromFile("char_sprites\\sp2_sprite.png")) {
		std::cerr << "WHAT";
	} else {
		player_sprite.setTexture(texture);
		player_sprite.setPosition(sf::Vector2f(window.getView().getCenter().x - 37.5f, window.getView().getCenter().y - 55.0f));
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (!walk_textures[i][j].loadFromFile("char_sprites\\walk" + toString(i) + "-" + toString(j) + ".png")) {
				std::cerr << "ERROR";
			}
		}
	}

	if (!inventory_texture.loadFromFile("assets_sprites\\inv-slot.png")) {
		
	} else {
		for (int i = 0; i < INV_SIZE; ++i) {
			inventory[i].setTexture(inventory_texture);
			inventory[i].setPosition((desktop_width - 70 * INV_SIZE) / 2 + 70 * i, 0.95 * desktop_height - 70);
		}
	}

	if (!chest_texture.loadFromFile("assets_sprites\\chest.png")) {

	} else {
		chest.setTexture(chest_texture);
		chest.setPosition(rand() % ((int)desktop_width - CHESTX), rand() % ((int)desktop_height - CHESTY));
	}

	if (!dialog_texture.loadFromFile("assets_sprites\\dialogue.png")) {

	}
	else {
		dialog.setTexture(dialog_texture);
		dialog.setPosition(chest.getPosition().x + 10, chest.getPosition().y - 40);
	}

	initPlayer();
	initChest();

	float x = player_sprite.getPosition().x;
	float y = player_sprite.getPosition().y;
	int currentPos[4] = { 0, 0, 0, 0 };
	sf::Clock clock;
	sf::Clock clockWalking;

	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				for (int i = 0; i < 4; ++i)
					if (event.key.code == keys[i]) {
						pressed[i] = (event.type ==sf::Event::KeyPressed);
						break;
					}
			}
		}

		if (clock.getElapsedTime().asMilliseconds() > 10) {
			bool keyPress = false;
			int lastKeyPressed = -1;
			for (int i = 0; i < 4; ++i) {
				if (pressed[i]) {
					x += displacement[i][0];
					y += displacement[i][1];
					keyPress = true;
					lastKeyPressed = i;
				}
			}

			x = max(0, x);
			x = min(x, desktop_width - PLAYERX);
			y = max(0, y);
			y = min(y, inventory[0].getPosition().y - PLAYERY);

			player_sprite.setPosition(x, y);
			if (keyPress && clockWalking.getElapsedTime().asMilliseconds() > 150) {
				for (int i = 0; i < 4; ++i) {
					if (pressed[i]) {
						currentPos[i] = (currentPos[i] + 1) % 4;
					}
				}

				player_sprite.setTexture(walk_textures[lastKeyPressed][currentPos[lastKeyPressed]]);
				clockWalking.restart();
			}

			window.clear();
			//window.draw(background);
			window.draw(player_sprite);
			for (int i = 0; i < INV_SIZE; ++i) {
				window.draw(inventory[i]);
			}
			initPlayer();
			window.draw(chest);
			if (checkOverlap(player, chestR)) {
				window.draw(dialog);
			}
			window.display();
			clock.restart();
		}
	}

	return 0;
}
