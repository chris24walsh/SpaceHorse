#ifndef SHIP_H
#define SHIP_H

#include "Game.h"

class Ship {
private:
	int m_id,
		m_gridX,
		m_gridY,
		m_height,
		m_width,
		m_speed,
		m_speedX,
		m_speedY,
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
		m_fireAngle[static_cast<int>(Fire::MAXFIREBALLS)];
	std::string m_shipSprite,
		m_shipSprite1,
		m_shipSprite2,
		m_shipSpriteCurrent,
		m_fireSprite; //don't need array for this, since display will take care of that.
	bool m_canDock;

public:
	Ship();
	Ship(std::string shipSprite, std::string shipSprite1, std::string shipSprite2, std::string fireSprite);
	void setFireCycle(int fireCycle);
	~Ship();
};

#endif