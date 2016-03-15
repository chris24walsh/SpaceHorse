#ifndef SPACE_LOGIC_H
#define SPACE_LOGIC_H

#include "Header.h"

#include "Space_display.h"
#include "Player.h"
#include "Map.h"

class Space_logic
{
private:
	Space_display *m_space;
	std::vector<Player> *m_players;
	Map *m_map;
	bool m_leftPressed,
		m_rightPressed,
		m_upPressed,
		m_downPressed,
		m_firePressed,
		m_gameOver,
		m_hyperDrive,
		m_paused,
		m_collided,
		m_angleAligned,
		m_distanceAligned,
		m_textEntered;
	int m_newKey,
		m_distance,
		m_distanceTravelX,
		m_distanceTravelY,
		m_x2,
		m_y2,
		m_maxX,
		m_maxY,
		m_hyperSpeed;
	std::stringstream m_enterCoordinates;
	std::string m_editText;
	double m_oldAngle,
		m_newAngle;

public:
	Space_logic();
	void load(Space_display &space, int windowWidth, int windowHeight, std::vector<Player> &players, Map &map);
	void unload();
	void update();
	void fire();
	int dock();
	void triggerCollision();
	void hyperDrive();
	int keyPress(ALLEGRO_EVENT &keyPressed);
	void keyRelease(ALLEGRO_EVENT &keyReleased);
	~Space_logic(void);
};

#endif