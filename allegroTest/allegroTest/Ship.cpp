#include "Ship.h"

Ship::Ship()
	:m_gridX(489000 + rand()%100),
	m_gridY(503800 + rand()%100),
	m_shipSprite("c:/dev/allegro/images/shipSprite.png"), //display will load these when needed, ship just tells it where to load from
	m_shipSprite1("c:/dev/allegro/images/shipSprite1.png"),
	m_shipSprite2("c:/dev/allegro/images/shipSprite2.png"),
	m_fireSprite("c:/dev/allegro/images/fireball.png"),
	//logic will set m_height and m_width later when it tells display to load these bitmaps
	m_maxSpeed(10),
	m_health(6),
	m_fireWidth(30), //maybe also set these to height and width of fireball bitmap?
	m_fireHeight(20),
	m_maxFireBalls(static_cast<int>(Fire::MAXFIREBALLS)),
	m_fireCycle(100)
{
	m_fireSpeed = m_maxSpeed + 10;
	for (int i=0;i<m_maxFireBalls;++i) 
	{
		m_fireX[i] = 2000;
		m_fireY[i] = 2000;
	}
}

Ship::Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite)
	:m_id(rand()%1000), //why this? it won't prevent a ship having the same id
	m_gridX(489000 + rand()%100),
	m_gridY(503800 + rand()%100),
	m_shipSprite(shipSprite), //display will load these when needed, ship just tells it where to load from
	m_shipSprite1(shipSprite1),
	m_shipSprite2(shipSprite2),
	m_fireSprite(fireSprite),
	//logic will set m_height and m_width later when it tells display to load these bitmaps
	m_maxSpeed(10),
	m_health(6),
	m_fireWidth(30), //maybe also set these to height and width of fireball bitmap?
	m_fireHeight(20),
	m_maxFireBalls(static_cast<int>(Fire::MAXFIREBALLS)),
	m_fireCycle(100)
{
	srand(time(NULL)); //what is this for?
	m_fireSpeed = m_maxSpeed + 10;
	for (int i=0;i<m_maxFireBalls;++i) 
	{
		m_fireX[i] = 2000;
		m_fireY[i] = 2000;
	}
}

void Ship::setFireCycle(int fireCycle) { m_fireCycle = fireCycle; }

Ship::~Ship(void)
{
}