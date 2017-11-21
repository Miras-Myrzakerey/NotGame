#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Object.h"
#include "GameWindow.h"

#define SPRITE_SPEED 3
#define INV_SIZE 10
#define PLAYERX 75
#define PLAYERY 110
#define CHESTX 70
#define CHESTY 86
#define EPS 1e-7


int keys[4] = { sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Up, sf::Keyboard::Down };
std::vector<int> frames = { 4, 4, 4, 4 };
bool pressed[4];
int desktopWidth, desktopHeight;
sf::Sprite inventory[INV_SIZE];
sf::Sprite dialog;
Player player(frames, 75, 110);
Player ghost(frames, 75, 110);
std::vector<std::string> lastSprites = {"char_sprites\\last_left.png", "char_sprites\\last_right.png" , "char_sprites\\last_up.png" , "char_sprites\\last_down.png" };
Object chest;

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

void initPlayer(Player &player) {
	float x = player.getPosition().x;
	float y = player.getPosition().y;
	player.initiateHitbox(Point(x, y), Point(x + PLAYERX, y + PLAYERY));
	player.initSprite("char_sprites\\player_sprite.png");
	player.setPosition(sf::Vector2f(0, 0));
	player.setLastSprites(lastSprites);
	player.setSpeed(15);
}

void initChest() {
	chest.setPosition(sf::Vector2f(rand() % desktopWidth, rand() % desktopHeight));
	float x = chest.getPosition().x;
	float y = chest.getPosition().y;
	chest.initSprite("assets_sprites\\ch1.png");
	chest.initiateHitbox(Point(x, y), Point(x + CHESTX, y + CHESTY));
}

int main()
{
	//	freopen("out", "w", stdout);
	srand(time(NULL));
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	desktopHeight = 600;
	desktopWidth = 600;
	GameWindow mainWindow(sf::VideoMode::VideoMode(500, 500), "Not Game", sf::Style::Titlebar | sf::Style::Close);

	std::vector<Object> objects;
	objects.push_back(player);
	sf::View mainView(sf::FloatRect(0, 0, 600, 600));
	mainWindow.changeView(mainView);
	sf::Event event;
	int lastKeyPressed[2] = { sf::Keyboard::Down, sf::Keyboard::Down };
	bool ghostMode = false;
	initPlayer(player);
	initPlayer(ghost);
	initChest();
	ghost.setPosition(sf::Vector2f(player.getPosition().x + 30, player.getPosition().y - 55));

	while (mainWindow.window.isOpen()) {
		while (mainWindow.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				mainWindow.window.close();
			}

			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					mainWindow.window.close();
				}

				for (int i = 0; i < 4; ++i) {
					if (event.key.code == keys[i]) {
						pressed[i] = true;
						lastKeyPressed[ghostMode == true] = event.key.code;
						break;
					}
				}

				if (event.key.code == sf::Keyboard::E) {
					ghostMode = true;
				}
			}

			if (event.type == sf::Event::KeyReleased) {
				for (int i = 0; i < 4; ++i) {
					if (event.key.code == keys[i]) {
						pressed[i] = false;
						break;
					}
				}

				if (event.key.code == sf::Keyboard::E) {
					ghostMode = false;
				}
			}
		}
		
		bool ok = false;
		for (int i = 0; i < 4; ++i) {
			if (pressed[i]) {
				ok = true;
				if (!ghostMode)
					player.handleMovement(keys[i], desktopWidth, desktopHeight);
				else
					ghost.handleMovement(keys[i], desktopWidth, desktopHeight);
				std::cout << i << '\n';
			}
		}

		if (!ghostMode) {
			ghost.setPosition(sf::Vector2f(player.getPosition().x + 30, player.getPosition().y - 55));
		}

		if (!ok) {
			if (!ghostMode)
				player.stayIdle(lastKeyPressed[0]);
			else
				ghost.stayIdle(lastKeyPressed[1]);
		} else {

		}

		objects.clear();
		objects.push_back(player);
		if (ghostMode) {
			objects.push_back(ghost);
		}
		objects.push_back(chest);
		if (player.checkCollision(chest)) {
			std::cout << "PLAYER\n";
		}

		if (ghost.checkCollision(chest)) {
			std::cout << "GHOST\n";
		}

		//mainView.setCenter(player.getPosition().x + 37.5, player.getPosition().y + 55);
		mainWindow.changeView(mainView);
		mainWindow.clearAndRedraw(objects);
	}
	return 0;
}
