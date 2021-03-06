#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Object.h"
#include "GameWindow.h"
#include "Chest.h"

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
std::vector<std::string> lastSprites = {"char_sprites\\last_left.png", "char_sprites\\last_right.png" , "char_sprites\\last_up.png" , "char_sprites\\last_down.png" };

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
	player.setSpeed(10);
}

void initChest(Object &chest, std::string file) {
	chest.setPosition(sf::Vector2f(rand() % desktopWidth, rand() % desktopHeight));
	float x = chest.getPosition().x;
	float y = chest.getPosition().y;
	chest.initSprite(file);
	chest.initiateHitbox(Point(x, y), Point(x + CHESTX, y + CHESTY));
}

bool chestsCollide(std::vector<Chest*> &chests) {
	for (int i = 0; i < chests.size(); ++i) {
		for (int j = i + 1; j < chests.size(); ++j) {
			if ((*(chests[i])).checkCollision(*chests[j])) {
				return true;
			}
		}
	}

	return false;
}

struct Light
{
	sf::Vector2f position;
	sf::Vector2f scale;
	sf::Color color;
	Light(sf::Vector2f nposition, sf::Vector2f nscale, sf::Color ncolor) :
		position(nposition),
		scale(nscale),
		color(ncolor)
	{
	}
};


void drawLight(sf::RenderWindow &window, const float x, const float y) {

}

int main()
{
	Player player(frames, 75, 110);
	Player ghost(frames, 75, 110);
	Chest chest(70, 86);
	Chest ghostChest(70, 86);
	Chest winChest(70, 86);
	std::vector<Chest*> chests;
	chests.push_back(&chest);
	chests.push_back(&ghostChest);
	chests.push_back(&winChest);
	//	freopen("out", "w", stdout);
	srand(time(NULL));
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	desktopHeight = 600;
	desktopWidth = 600;
	GameWindow mainWindow(sf::VideoMode::VideoMode(600, 600, 32), "Not Game", sf::Style::Titlebar | sf::Style::Close);

	std::vector<Object> objects;
	objects.push_back(player);
	sf::View mainView(sf::FloatRect(0, 0, 600, 600));
	mainWindow.changeView(mainView);
	sf::Event event;
	int lastKeyPressed[2] = { sf::Keyboard::Down, sf::Keyboard::Down };
	bool ghostMode = false;
	initPlayer(player);
	initPlayer(ghost);
	//ghost.setSpeed(40);
	do {
		initChest(chest, "assets_sprites\\chest.png");
		initChest(ghostChest, "assets_sprites\\chest.png");
		initChest(winChest, "assets_sprites\\win_chest.png");
	} while (chestsCollide(chests));
	ghost.setPosition(player.getPosition());
	bool collided[3] = { false, false, false };

	sf::RenderTexture tex;
	tex.create(mainWindow.window.getSize().x, mainWindow.window.getSize().y);
	std::vector<sf::Vector2f> lights;

	const sf::Color colors[3] = { sf::Color::Red, sf::Color::Green, sf::Color::Blue };
	
	while (mainWindow.window.isOpen()) {
		while (mainWindow.window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				mainWindow.window.close();
			}

			if (event.type == sf::Event::MouseButtonPressed)
				lights.push_back(mainWindow.window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)));

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
				//std::cout << i << '\n';
			}
		}

		if (!ghostMode) {
			ghost.setPosition(player.getPosition());
		}

		if (!ok) {
			if (!ghostMode)
				player.stayIdle(lastKeyPressed[0]);
			else
				ghost.stayIdle(lastKeyPressed[1]);
		}

		objects.clear();
		objects.push_back(player);
		if (ghostMode) {
			objects.push_back(ghost);
		}
		objects.push_back(chest);
		objects.push_back(ghostChest);
		objects.push_back(winChest);
		if (player.checkCollision(chest)) {
//			std::cout << "PLAYER\n";
			if (!collided[0]) {
				collided[0] = true;
				chest.changeState();
			}
		}
		else {
			if (collided[0]) {
				collided[0] = false;
				chest.changeState();
			}
		}

		if (ghostMode && ghost.checkCollision(ghostChest)) {
//			std::cout << "GHOST\n";
			if (!collided[1]) {
				collided[1] = true;
				ghostChest.changeState();
			}
		}
		else {
			if (collided[1]) {
				collided[1] = false;
				ghostChest.changeState();
			}
		}

		if (collided[0] && collided[1]) {
			if (!collided[2]) {
				winChest.changeState();
				collided[2] = true;
			}
		}
		else {
			if (collided[2]) {
				collided[2] = false;
				winChest.changeState();
			}
		}
		

		//mainView.setCenter(player.getPosition().x + 37.5, player.getPosition().y + 55);
		mainWindow.changeView(mainView);
		//mainWindow.clearAndRedraw(objects);

		mainWindow.window.clear();
		tex.clear(sf::Color::Black);

		sf::CircleShape sha;
		sha.setRadius(150.f);
		sha.setOrigin(sha.getRadius(), sha.getRadius());
		sha.setPosition(mainWindow.window.mapPixelToCoords(sf::Mouse::getPosition(mainWindow.window)));
		tex.draw(sha, sf::BlendAdd);
		for (int i = 0; i < lights.size(); ++i)
		{
			sha.setPosition(lights[i]);
			sha.setFillColor(colors[i % 3]);
			tex.draw(sha, sf::BlendAdd);
		}
		tex.display();

		//lit scene
		mainWindow.drawObjects(objects);

		//multiply by light
		mainWindow.window.draw(sf::Sprite(tex.getTexture()), sf::BlendMultiply);
		mainWindow.window.display();
	}
	return 0;
}
