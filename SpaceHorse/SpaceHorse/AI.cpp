#include "AI.h"

AI::AI()
	:m_counter(0)
{
	getShip().setX(501000);
	getShip().setY(501000);
}

Ship & AI::getShip()
{
	return ship;
}

void AI::moveAI()
{
	if (m_counter < 250) {
		getShip().setX(
	}
}

AI::~AI()
{
}
