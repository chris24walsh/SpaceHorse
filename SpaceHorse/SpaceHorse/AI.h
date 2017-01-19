#pragma once
#include "Header.h"
#include "Animation.h"
#include "Ship.h"

class AI {
private:
	Ship ship;
	int m_counter;
public:
	AI();
	Ship& getShip();
	void moveAI();
	~AI();
};