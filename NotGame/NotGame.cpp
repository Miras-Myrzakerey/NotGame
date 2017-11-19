#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>

#define SPRITE_SPEED 3

float min(float a, float b) {
	return a < b ? a : b;
}

float max(float a, float b) {
	return a > b ? a : b;
}

int keys[4] = { sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Down, sf::Keyboard::Up };
int displacement[4][2] = { {-SPRITE_SPEED, 0}, {SPRITE_SPEED, 0}, {0, SPRITE_SPEED}, {0, -SPRITE_SPEED} };
bool pressed[4];

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

int main()
{
	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "SFML works!", sf::Style::Fullscreen);
	sf::Texture texture, background_texture;
	sf::Sprite sprite, background;
	sf::Vector2f targetSize(1920, 1080);
	sf::Texture walk_textures[4][4];

	if (!texture.loadFromFile("char_sprites\\sp2_sprite.png")) {
		std::cerr << "WHAT";
	} else {
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(window.getView().getCenter().x - 37.5f, window.getView().getCenter().y - 55.0f));
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (!walk_textures[i][j].loadFromFile("char_sprites\\walk" + toString(i) + "-" + toString(j) + ".png")) {
				std::cerr << "ERROR";
			}
		}
	}

	if (!background_texture.loadFromFile("grass.jpg")) {
		std::cerr << "RIP";
	} else {
		background.setTexture(background_texture);
		//window.draw(background);
		background.setScale( targetSize.x / background.getLocalBounds().width, targetSize.y / background.getLocalBounds().height);
	}

	float x = sprite.getPosition().x;
	float y = sprite.getPosition().y;
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
			x = min(x, 1920);
			y = max(0, y);
			y = min(y, 1080);

			sprite.setPosition(x, y);
			if (keyPress && clockWalking.getElapsedTime().asMilliseconds() > 150) {
				for (int i = 0; i < 4; ++i) {
					if (pressed[i]) {
						currentPos[i] = (currentPos[i] + 1) % 4;
					}
				}

				sprite.setTexture(walk_textures[lastKeyPressed][currentPos[lastKeyPressed]]);
				clockWalking.restart();
			}

			window.clear();
			//window.draw(background);
			window.draw(sprite);
			window.display();
			clock.restart();
		}
	}

	return 0;
}
