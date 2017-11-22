#include "stdafx.h"
#include "Chest.h"

Chest::Chest()
{
}

Chest::~Chest()
{
}

Chest::Chest(int spW, int spH) {
	spriteWidth = spW;
	spriteHeight = spH;
}

void Chest::changeState() {
	state ^= 1;
	mainSprite.setTextureRect(sf::IntRect(state * spriteWidth, 0, spriteWidth, spriteHeight));
}
