#pragma once
#include "Object.h"
class Chest :
	public Object
{
public:
	Chest();
	Chest(int spW, int spH);
	~Chest();

	void changeState();
protected:
	int state; //0 open; 1 closed;
};

