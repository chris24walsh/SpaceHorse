#ifndef PLAYER_H
#define PLAYER_H

#include "Header.h"

#include "Ship.h"

class Player
{
private:
	Ship ship;

public:
	Player(void);
	Ship& getShip();
	~Player(void);
};

#endif