#include "stdafx.h"
#include "GameWindow.h"


GameWindow::GameWindow()
{
}

GameWindow::GameWindow(sf::VideoMode mode, std::string name)
:window(mode, name){
}

GameWindow::GameWindow(sf::VideoMode mode, std::string name, int style)
:window(mode, name, style) {
}

GameWindow::~GameWindow()
{
}

void GameWindow::drawObjects(std::vector<Object> drawables)
{
	for (Object cur : drawables) {
		window.draw(cur.getSprite());
	}
}

void GameWindow::clearAndRedraw(std::vector<Object> worldObjects)
{
	window.clear();
	this->drawObjects(worldObjects);
	window.display();
}

void GameWindow::changeView(sf::View newView)
{
	window.setView(newView);
}
