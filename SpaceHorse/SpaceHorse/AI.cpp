#include "AI.h"

AI::AI()
	:ship(),
	m_counter(0)
{
	getShip().setX(512200);
	getShip().setY(500000);
}

Ship & AI::getShip()
{
	return ship;
}

void AI::moveAI()
{
	if (1) { //Always, move in straight line
		getShip().setX(getShip().getX() + 1);
	}
}

AI::~AI()
{
}
