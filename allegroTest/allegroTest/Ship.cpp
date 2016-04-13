#include "Ship.h"

Ship::Ship()
	:m_speed(0),
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
	m_currentSprite(0),
	m_shipSprite(0),
	m_maxSpeed(10),
	m_health(6),
	//logic will also set height and width of fireball to bitmap when loading display
	m_fireSprite("c:/dev/allegro/images/fireball.png"),
	m_maxFireBalls(static_cast<int>(Fire::MAXFIREBALLS)),
	m_fireCycle(100),
	m_dockPlanet(0),
	m_hyperDistanceX(0),
	m_hyperDistanceY(0),
	m_hyperX(0),
	m_hyperY(0),
	m_hyperSpeed(50),
	m_newAngle(0)
{
	m_shipSprite.reserve(3);
	m_shipSprite.push_back(al_load_bitmap("c:/dev/allegro/images/shipSprite.png"));
	m_shipSprite.push_back(al_load_bitmap("c:/dev/allegro/images/shipSprite1.png"));
	m_shipSprite.push_back(al_load_bitmap("c:/dev/allegro/images/shipSprite2.png"));
	for(int i=0; i<m_shipSprite.size(); ++i)
	{ if(!m_shipSprite.at(i)) { printf("%s \n", "Failed to load the shipSprite image"); _getch(); exit(1); } }

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

void Ship::animateRocket()
{
	if(m_flipflop<5)
	{ m_currentSprite = 1; }
	else
	{ m_currentSprite = 2; }
	++m_flipflop; //increase by one
	if (m_flipflop==10)
	{ m_flipflop = 0; } //reset when it reaches 10
}

void Ship::move(bool x, bool y)
{
	if(x) { m_coordinates.x += m_speedX; }
	if(y) { m_coordinates.y += m_speedY; }
	//animate if moving
	if (m_speed>0) { animateRocket(); }
	else { m_currentSprite = 0; }
}

void Ship::turn(bool left)
{
	if(left)
	{ m_angle -= 0.05; }
	else
	{ m_angle += 0.05; }
	//keep angle above 0 and below 6.28
	if (m_angle>6.25) { m_angle = 0; }
	if (m_angle<0) { m_angle = 6.25; }
	updateSpeedXY();
}

void Ship::accelerate(bool faster)
{
	if(faster && m_speed<m_maxSpeed)
	{ ++m_speed; }
	else if(m_speed>0)
	{ --m_speed; }
	updateSpeedXY();
}

void Ship::beginHyperDrive(int hyperX, int hyperY)
{
	m_hyperX = hyperX;
	m_hyperY = hyperY;

	m_newAngle = atan2( (m_hyperY - m_coordinates.y),  (m_hyperX - m_coordinates.x) );
	if (m_newAngle<0) { m_newAngle += 3.14*2; }

	m_hyperDistanceX = abs(abs(m_hyperX) - abs(m_coordinates.x));
	m_hyperDistanceY = abs(abs(m_hyperY) - abs(m_coordinates.y));
}

void Ship::hyperDrive()
{
	if(m_angle!=m_newAngle) { hyperAlignAngle(); }
	else { hyperMove(); }
}

void Ship::hyperAlignAngle()
{
	//Decide if better to rotate clockwise or anticlockwise
	if (m_newAngle > m_angle)
	{
		if (m_newAngle - m_angle < 3.14)
		{ turn(false); }
		else
		{ turn(true); }
	}
	else
	{
		if (m_angle - m_newAngle > 3.14)
		{ turn(false); }
		else
		{ turn(true); }
	}
	//Check for nearness then align angle exactly
	if (abs(abs(m_angle) - abs(m_newAngle)) < 0.05)
	{ m_angle = m_newAngle; }
}

void Ship::hyperMove()
{
	m_coordinates.x += m_maxSpeed * m_hyperSpeed * cos(m_angle);
	m_coordinates.y += m_maxSpeed * m_hyperSpeed * sin(m_angle);
	m_hyperDistanceX -= abs(m_maxSpeed * m_hyperSpeed * cos(m_angle));
	m_hyperDistanceY -= abs(m_maxSpeed * m_hyperSpeed * sin(m_angle));

	//if ( abs(abs(players.at(0).x) - abs(x2)) < 1000 && abs(abs(players.at(0).y) - abs(y2)) < 1000) { //Check for nearness, then align distance exactly
	if (m_hyperDistanceX <= 1000 && m_hyperDistanceY <= 1000)
	{
		m_coordinates.x = m_hyperX;
		m_coordinates.y = m_hyperY;
		setDocked(false, 0);
	}
}

void Ship::shutdown()
{
	for(int i=0; i<m_shipSprite.size(); ++i)
	{ if(m_shipSprite.at(i)) { al_destroy_bitmap(m_shipSprite.at(i)); } }
}

Ship::~Ship(void)
{
}