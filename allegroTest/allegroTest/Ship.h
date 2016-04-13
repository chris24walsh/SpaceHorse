#ifndef SHIP_H
#define SHIP_H

#include "Header.h"

class Ship {
private:
	int m_speed,
		m_maxSpeed,
		m_health,
		m_flipflop,
		m_currentSprite,
		m_fireballNumber,
		m_maxFireBalls,
		m_fireX[static_cast<int>(Fire::MAXFIREBALLS)], //if using dynamically allocated arrays here, do it right (or use vectors instead)
		m_fireY[static_cast<int>(Fire::MAXFIREBALLS)],
		m_fireHeight,
		m_fireWidth,
		m_fireSpeed,
		m_fireCycle,
		m_dockPlanet,
		m_hyperDistanceX,
		m_hyperDistanceY,
		m_hyperX,
		m_hyperY,
		m_hyperSpeed;
	double m_angle,
		m_newAngle,
		m_speedX,
		m_speedY,
		m_fireAngle[static_cast<int>(Fire::MAXFIREBALLS)];
	Coordinates m_coordinates;
	std::vector<ALLEGRO_BITMAP*> m_shipSprite;
	std::string m_fireSprite; //don't need array for this, since display will take care of that.
	bool m_upgraded,
		m_docked;

public:
	Ship(); //we can combine these constructors to load from a textfile.
	Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite);
	
	void setFireCycle(int fireCycle) { m_fireCycle = fireCycle; }
	int getFireCycle() { return m_fireCycle; }
	void damage(int damage) { m_health -= damage; if(m_health<0) { m_health = 0; } }
	int getHealth() { return m_health; }
	void setDocked(bool docked, int dockPlanet) { m_docked = docked; if(m_docked) { m_dockPlanet = dockPlanet; stop(); } }
	int getDockPlanet() { return m_dockPlanet; }
	bool getDocked() { return m_docked; }

	ALLEGRO_BITMAP* getShipSprite() { return m_shipSprite.at(m_currentSprite); }
	int getWidth() { return al_get_bitmap_width(m_shipSprite.at(0)); }
	int getHeight() { return al_get_bitmap_height(m_shipSprite.at(0)); }

	//fireball functions
	void setUpgraded(std::string fireSprite) { m_fireSprite = fireSprite; m_upgraded = true; }
	bool getUpgraded() { return m_upgraded; }
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
	const Coordinates& getCoordinates() { return m_coordinates; }
	void move(bool x, bool y);
	void animateRocket();
	void setX(double x) { m_coordinates.x = x; }
	void setY(double y) { m_coordinates.y = y; }
	double getAngle() { return m_angle; }
	void turn(bool left);
	int getSpeed() { return m_speed; }
	double getSpeedX() { return m_speedX; }
	double getSpeedY() { return m_speedY; }
	int getMaxSpeed() { return m_maxSpeed; }
	void accelerate(bool faster);
	void updateSpeedXY() { m_speedX = m_speed * cos(m_angle); m_speedY = m_speed * sin(m_angle); }
	void stop() { m_speed = m_speedX = m_speedY = m_flipflop = m_currentSprite = 0; }

	//hyperdrive
	void beginHyperDrive(int hyperX, int hyperY);
	void hyperDrive();
	void hyperAlignAngle();
	void hyperMove();

	void shutdown();
	~Ship();
};

#endif