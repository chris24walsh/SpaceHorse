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
	m_x(489000 + rand()%100),
	m_y(503800 + rand()%100),
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
	m_x(489000 + rand()%100),
	m_y(503800 + rand()%100),
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
	m_fireSpeed = m_maxSpeed + 10;
	for (int i=0;i<m_maxFireBalls;++i)
	{
		m_fireX[i] = 2000;
		m_fireY[i] = 2000;
		m_fireAngle[i] = 0;
	}
}


void Ship::setDocked(bool docked, int dockPlanet)
{
	m_docked = docked;
	if(m_docked)
	{ m_dockPlanet = dockPlanet; m_speed = 0; }
}

void Ship::setFireCycle(int fireCycle) { m_fireCycle = fireCycle; }
int Ship::getFireCycle() { return m_fireCycle; }
void Ship::setShipSize(int shipWidth, int shipHeight) { m_width = shipWidth; m_height = shipHeight; }
void Ship::setFireSize(int fireWidth, int fireHeight) { m_fireWidth = fireWidth; m_fireHeight = fireHeight; }
void Ship::setUpgraded(std::string fireSprite) { m_fireSprite = fireSprite; m_upgraded = true; }
bool Ship::getUpgraded() { return m_upgraded; }
std::string Ship::getShipSprite() { return m_shipSprite; }
std::string Ship::getShipSprite1() { return m_shipSprite1; }
std::string Ship::getShipSprite2() { return m_shipSprite2; }
std::string Ship::getFireSprite() { return m_fireSprite; }
int Ship::getMaxFireBalls() { return m_maxFireBalls; }
int Ship::getFireBallNumber() { return m_fireballNumber; }
void Ship::setFireBallNumber(int fireBallNumber) { m_fireballNumber = fireBallNumber; }
int Ship::getFireX(int index) { return m_fireX[index]; }
int Ship::getFireY(int index) { return m_fireY[index]; }
void Ship::setFireX(int index, int fireX) { m_fireX[index] = fireX; }
void Ship::setFireY(int index, int fireY) { m_fireY[index] = fireY; }
void Ship::updateFireX(int index) { m_fireX[index] += m_fireSpeed * cos(m_fireAngle[index]); }
void Ship::updateFireY(int index) { m_fireY[index] += m_fireSpeed * sin(m_fireAngle[index]); }
double Ship::getFireAngle(int index) { return m_fireAngle[index];}
void Ship::setFireAngle(int index, double fireAngle) { m_fireAngle[index] = fireAngle; }
double Ship::getX() { return m_x; }
double Ship::getY() { return m_y; }
void Ship::setX(double x) { m_x = x; }
void Ship::setY(double y) { m_y = y; }
double Ship::getAngle() { return m_angle; }
void Ship::setAngle(double angle) { m_angle = angle; }
void Ship::setHealth(int health) { if(health>0) {m_health = health;} else m_health=0; }
int Ship::getHealth() { return m_health; }
int Ship::getWidth() { return m_width; }
int Ship::getHeight() { return m_height; }
int Ship::getSpeed() { return m_speed; }
int Ship::getMaxSpeed() { return m_maxSpeed; }
double Ship::getSpeedX() { return m_speedX; }
double Ship::getSpeedY() { return m_speedY; }
void Ship::setSpeedX(double speedX) { m_speedX = speedX; }
void Ship::setSpeedY(double speedY) { m_speedY = speedY; }
int Ship::getFlipflop() { return m_flipflop; }
void Ship::setFlipflop(int flipflop) { m_flipflop = flipflop; }
bool Ship::goingMaxSpeed() { return (m_speed==m_maxSpeed); }
void Ship::setSpeed(int speed) { m_speed = speed; }
int Ship::getDockPlanet() { return m_dockPlanet; }
bool Ship::getDocked() { return m_docked; }
std::vector<Animation> Ship::getAnimations() { return m_animations ; }
void Ship::setAnimations(Animation a1) { m_animations.push_back(a1); }
Ship::~Ship(void)
{
}