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
		m_fireCycle;
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
	bool m_canDock,
		m_upgraded,
		m_docked;

public:
	Ship();
	Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite);
	void setFireCycle(int fireCycle);
	int getFireCycle();
	void setShipSize(int shipWidth, int shipHeight);
	void setFireSize(int fireWidth, int fireHeight);
	void setUpgraded(std::string fireSprite);
	bool getUpgraded();
	bool getCanDock();
	void toggleDocked();
	std::string getShipSprite();
	std::string getShipSprite1();
	std::string getShipSprite2();
	std::string getFireSprite();
	int getMaxFireBalls();
	int getFireBallNumber();
	void setFireBallNumber(int fireBallNumber);
	int getFireX(int index);
	int getFireY(int index);
	void setFireX(int index, int fireX);
	void setFireY(int index, int fireY);
	void updateFireX(int index);
	void updateFireY(int index);
	double getFireAngle(int index);
	void setFireAngle(int index, double fireAngle);
	double getX();
	double getY();
	void setX(double x);
	void setY(double y);
	double getAngle();
	void setAngle(double angle);
	void setHealth(int health);
	int getHealth();
	int getWidth();
	int getHeight();
	int getSpeed();
	int getMaxSpeed();
	double getSpeedX();
	double getSpeedY();
	void setSpeedX(double speedX);
	void setSpeedY(double speedY);
	int getFlipflop();
	void setFlipflop(int flipflop);
	bool goingMaxSpeed();
	void setSpeed(int speed);
	void setCanDock(bool canDock);
	~Ship();
};

#endif