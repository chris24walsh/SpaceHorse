#ifndef SHIP_H
#define SHIP_H

#include "Header.h"

class Ship {
private:
	int m_id,
		m_height,
		m_width,
		m_speed,
		m_maxSpeed,
		m_health,
		m_flipflop,
		m_fireballNumber,
		m_maxFireBalls,
		m_fireX[static_cast<int>(Fire::MAXFIREBALLS)], //if using dynamically allocated arrays here, do it right (or use vectors instead)
		m_fireY[static_cast<int>(Fire::MAXFIREBALLS)],
		m_fireHeight,
		m_fireWidth,
		m_fireSpeed,
		m_fireCycle,
		m_dockPlanet;
	double m_angle,
		m_speedX,
		m_speedY,
		m_x,
		m_y,
		m_fireAngle[static_cast<int>(Fire::MAXFIREBALLS)];
	std::string m_shipSprite,
		m_shipSprite1,
		m_shipSprite2,
		m_fireSprite; //don't need array for this, since display will take care of that.
	bool m_upgraded,
		m_docked;

public:
	Ship(); //we can combine these constructors to load from a textfile.
	Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite);
	
	void setUpgraded(std::string fireSprite) { m_fireSprite = fireSprite; m_upgraded = true; }
	bool getUpgraded() { return m_upgraded; }
	void setFireCycle(int fireCycle) { m_fireCycle = fireCycle; }
	int getFireCycle() { return m_fireCycle; }
	void setHealth(int health) { if(health>0) {m_health = health;} else m_health=0; }
	int getHealth() { return m_health; }
	void setDocked(bool docked, int dockPlanet) { m_docked = docked; if(m_docked) { m_dockPlanet = dockPlanet; m_speed = 0; } }
	int getDockPlanet() { return m_dockPlanet; }
	bool getDocked() { return m_docked; }

	//put these into one vector of Allegro bitmaps for ship images
	std::string getShipSprite() { return m_shipSprite; }
	std::string getShipSprite1() { return m_shipSprite1; }
	std::string getShipSprite2() { return m_shipSprite2; }
	void setShipSize(int shipWidth, int shipHeight) { m_width = shipWidth; m_height = shipHeight; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	int getFlipflop() { return m_flipflop; }
	void setFlipflop(int flipflop) { m_flipflop = flipflop; }

	//fireball functions
	std::string getFireSprite() { return m_fireSprite; }
	void setFireSize(int fireWidth, int fireHeight) { m_fireWidth = fireWidth; m_fireHeight = fireHeight; }
	int getMaxFireBalls() { return m_maxFireBalls; }
	int getFireBallNumber() { return m_fireballNumber; }
	void setFireBallNumber(int fireBallNumber) { m_fireballNumber = fireBallNumber; }
	int getFireX(int index) { return m_fireX[index]; }
	int getFireY(int index) { return m_fireY[index]; }
	void setFireX(int index, int fireX) { m_fireX[index] = fireX; }
	void setFireY(int index, int fireY) { m_fireY[index] = fireY; }
	void updateFireX(int index) { m_fireX[index] += m_fireSpeed * cos(m_fireAngle[index]); }
	void updateFireY(int index) { m_fireY[index] += m_fireSpeed * sin(m_fireAngle[index]); }
	double getFireAngle(int index) { return m_fireAngle[index];}
	void setFireAngle(int index, double fireAngle) { m_fireAngle[index] = fireAngle; }

	//ship movement
	double getX() { return m_x; }
	double getY() { return m_y; }
	void setX(double x) { m_x = x; }
	void setY(double y) { m_y = y; }
	double getAngle() { return m_angle; }
	void setAngle(double angle) { m_angle = angle; }
	int getSpeed() { return m_speed; }
	int getMaxSpeed() { return m_maxSpeed; }
	double getSpeedX() { return m_speedX; }
	double getSpeedY() { return m_speedY; }
	void setSpeedX(double speedX) { m_speedX = speedX; }
	void setSpeedY(double speedY) { m_speedY = speedY; }
	bool goingMaxSpeed() { return (m_speed==m_maxSpeed); }
	void setSpeed(int speed) { m_speed = speed; }

	~Ship();
};

#endif