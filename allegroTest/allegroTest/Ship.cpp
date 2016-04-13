#include "Ship.h"

Ship::Ship()
	:m_id(0),
	m_height(0),
	m_width(0),
	m_speed(0),
	m_flipflop(0),
	m_fireballNumber(0),
	m_fireHeight(0),
	m_fireWidth(0),
	m_angle(0),
	m_speedX(0),
	m_speedY(0),
	m_upgraded(0),
	m_docked(0),
	m_coordinates(),
	m_shipSprite("c:/dev/allegro/images/shipSprite.png"), //display will load these when needed, ship just tells it where to load from
	m_shipSprite1("c:/dev/allegro/images/shipSprite1.png"),
	m_shipSprite2("c:/dev/allegro/images/shipSprite2.png"),
	m_fireSprite("c:/dev/allegro/images/fireball.png"),
	//logic will set m_height and m_width later when it tells display to load these bitmaps
	m_maxSpeed(10),
	m_health(6),
	//logic will also set height and width of fireball to bitmap when loading display
	m_maxFireBalls(static_cast<int>(Fire::MAXFIREBALLS)),
	m_fireCycle(100),
	m_dockPlanet(0)
{
	m_coordinates.x = 489000 + rand()%100;
	m_coordinates.y = 503800 + rand()%100;
	m_fireSpeed = m_maxSpeed + 10;
	for (int i=0;i<m_maxFireBalls;++i) 
	{
		m_fireX[i] = 2000;
		m_fireY[i] = 2000;
		m_fireAngle[i] = 0;
	}
}

Ship::Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite)
	:m_id(rand()%1000), 
/*Why this? It won't prevent a ship having the same id.
Also this % modulus method will not return a uniform distribution of pseudo-random numbers.
I.e. out of 32767, 1 extra for each number 1-767.
It will be weighted to lower numbers - a loaded dice (not just poor randomness, which in itself is not necessarily a problem).
But yes, rand() is pretty poor*/
	m_height(0),
	m_width(0),
	m_speed(0),
	m_flipflop(0),
	m_fireballNumber(0),
	m_fireHeight(0),
	m_fireWidth(0),
	m_angle(0),
	m_speedX(0),
	m_speedY(0),
	m_upgraded(0),
	m_docked(0),
	m_coordinates(),
	m_shipSprite(shipSprite), //display will load these when needed, ship just tells it where to load from
	m_shipSprite1(shipSprite1),
	m_shipSprite2(shipSprite2),
	m_fireSprite(fireSprite),
	//logic will set m_height and m_width later when it tells display to load these bitmaps
	m_maxSpeed(10),
	m_health(6),
	//logic will also set height and width of fireball to bitmap when loading display
	m_maxFireBalls(static_cast<int>(Fire::MAXFIREBALLS)),
	m_fireCycle(100),
	m_dockPlanet(0)
{
	m_coordinates.x = 489000 + rand()%100;
	m_coordinates.y = 503800 + rand()%100;
	m_fireSpeed = m_maxSpeed + 10;
	for (int i=0;i<m_maxFireBalls;++i)
	{
		m_fireX[i] = 2000;
		m_fireY[i] = 2000;
		m_fireAngle[i] = 0;
	}
}

void Ship::accelerate(bool accelerate)
{
	if(accelerate && m_speed<m_maxSpeed)
	{ ++m_speed; }
	else if(m_speed>0)
	{ --m_speed; }
	m_speedX = m_speed * cos(m_angle);
	m_speedY = m_speed * sin(m_angle);
}

Ship::~Ship(void)
{
}