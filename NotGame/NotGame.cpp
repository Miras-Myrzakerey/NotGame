#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SPRITE_SPEED 3
#define INV_SIZE 10
#define PLAYERX 75
#define PLAYERY 110
#define CHESTX 70
#define CHESTY 86
#define EPS 1e-7

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

bool greaterOrEqual(const float &a, const float &b) {
	if (fabs(a - b) < EPS)
		return true;

	return a - b > EPS;
}

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
		if (greaterOrEqual(b.x, leftUp.x) 
			&& greaterOrEqual(rightDown.x, b.x) 
			&& greaterOrEqual(b.y, leftUp.y)
			&& greaterOrEqual(rightDown.y, b.y)) {
			return true;
		}

		return false;
	}
}player, chestR;

template <typename T>
T min(const T &a, const T &b) {
	return a < b ? a : b;
}

template <typename T>
T max(const T &a, const T &b) {
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
//	freopen("out", "w", stdout);
	srand(time(NULL));
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	desktop_height = 1000;
	desktop_width = 1000;
	sf::RenderWindow window(sf::VideoMode::VideoMode(500, 500), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture texture, chest_texture1, chest_texture2;
	sf::Vector2f targetSize(desktop.width, desktop.height);
	sf::Texture walk_textures[4][4], inventory_texture, dialog_texture;
	sf::Texture background_texture;
	sf::Sprite background;
	sf::Texture looking_left, looking_right;

	if (!looking_left.loadFromFile("char_sprites\\sp23_sprite.png")) {

	}

	if (!looking_right.loadFromFile("char_sprites\\sp22_sprite.png")) {

	}

	if (!background_texture.loadFromFile("assets_sprites\\background.png")) {
	} else {
		background.setTexture(background_texture);
		background.setPosition(0, 0);
	}

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

	if (!chest_texture1.loadFromFile("assets_sprites\\ch1.png")) {

	} else {
		chest.setTexture(chest_texture1);
		std::cout << chest_texture1.getSize().x << ' ' << chest_texture1.getSize().y << "\n";
		chest.setPosition(rand() % ((int)desktop_width - CHESTX), rand() % ((int)desktop_height - CHESTY));
	}

	if (!chest_texture2.loadFromFile("assets_sprites\\ch2.png")) {

	} else {
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
	sf::Clock inv_clock;
	bool inv_switch = false;
	int cur_slot = 0;
	sf::Texture chosen_texture;
	sf::Sprite chosen_slot;
	sf::View view1(sf::FloatRect(200, 200, 400, 400));
	window.setView(view1);

	if (!chosen_texture.loadFromFile("assets_sprites\\chosen_slot.png")) {

	} else {
		chosen_slot.setTexture(chosen_texture);
		chosen_slot.setPosition(inventory[0].getPosition());
	}

	int lastKeyPressed = -1;

	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			// catch the resize events
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
			}

			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}

				for (int i = 0; i < 4; ++i)
					if (event.key.code == keys[i]) {
						pressed[i] = true;
						break;
					}
								 
				if (event.key.code == sf::Keyboard::E) {
					inv_switch = true;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				for (int i = 0; i < 4; ++i)
					if (event.key.code == keys[i]) {
						pressed[i] = false;
						break;
					}

				if (event.key.code == sf::Keyboard::E) {
					inv_switch = false;
				}
			}
		}

		if (clock.getElapsedTime().asMilliseconds() > 10) {
			bool keyPress = false;
			for (int i = 0; i < 4; ++i) {
				if (pressed[i]) {
					if (!inv_switch) {
						x += displacement[i][0];
						y += displacement[i][1];
						keyPress = true;
						lastKeyPressed = i;
					}
					else {
						if (inv_clock.getElapsedTime().asMilliseconds() > 75) {
							if (i == 0) {
								cur_slot = max(0, cur_slot - 1);
							}

							if (i == 1) {
								cur_slot = min(INV_SIZE - 1, cur_slot + 1);
							}

							chosen_slot.setPosition(inventory[cur_slot].getPosition());
							inv_clock.restart();
						}
					}
				}
			}

			x = max(0.0f, x);
			x = min(x, desktop_width - PLAYERX);
			y = max(0.0f, y);
			y = min(y, desktop_height - PLAYERY);

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

			if (!keyPress) {
				switch (lastKeyPressed) {
					case 0: player_sprite.setTexture(looking_left); break;
					case 1: player_sprite.setTexture(looking_right); break;
					case 2: player_sprite.setTexture(texture); break;
					case 3: player_sprite.setTexture(walk_textures[3][0]); break;
				}
			}

			window.clear();
			window.draw(background);
			view1 = window.getDefaultView();
			view1.setCenter(player_sprite.getPosition());
			window.setView(view1);
			window.draw(chest);
			window.draw(player_sprite);
			if (inv_switch) {
				for (int i = 0; i < INV_SIZE; ++i) {
					window.draw(inventory[i]);
				}
				window.draw(chosen_slot);
			}
			initPlayer();
			if (checkOverlap(player, chestR) || checkOverlap(chestR, player)	) {
				chest.setTexture(chest_texture2);
				window.draw(dialog);
			} else {
				chest.setTexture(chest_texture1);
			}
			/*sf::RectangleShape pl(sf::Vector2f(75, 110));
			sf::RectangleShape ch(sf::Vector2f(70, 70));
			pl.setPosition(player_sprite.getPosition());
			ch.setPosition(chest.getPosition());
			pl.setOutlineColor(sf::Color(0, 0, 0));
			ch.setOutlineColor(sf::Color(0, 0, 0));
			pl.setOutlineThickness(5);
			ch.setOutlineThickness(5);
			window.draw(pl);
			window.draw(ch);*/
			window.display();
			clock.restart();
			//if (event.key.code == sf::Keyboard::C) {

			//}
		}
	}

	return 0;
}
