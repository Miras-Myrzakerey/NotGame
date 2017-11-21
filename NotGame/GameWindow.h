#ifndef _GAMEWINDOW_H
#define _GAMEWINDOW_H

#include <SFML\Graphics.hpp>
#include "Object.h"
class GameWindow
{
public:
	GameWindow();
	GameWindow(sf::VideoMode, std::string name);
	GameWindow::GameWindow(sf::VideoMode mode, std::string name, int style);
	~GameWindow();

	void drawObjects(std::vector<Object> drawables);
	void clearAndRedraw(std::vector<Object> worldObjects);
	void changeView(sf::View newView);
	sf::RenderWindow window;
	sf::View playerView;
};

#endif