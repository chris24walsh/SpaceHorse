#ifndef SPACE_LOGIC_H
#define SPACE_LOGIC_H

#include "Header.h"

#include "Space_display.h"
#include "Player.h"
#include "AI.h"
#include "Map.h"
#include "Game_mode.h"

class Space_logic
{
private:
	Space_display *m_space_display;
	std::vector<Player> *m_players;
	std::vector<AI> *m_ai;
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
	std::string m_editText;
	double m_oldAngle,
		m_newAngle;
	ALLEGRO_SAMPLE *fireball;

public:
	Space_logic();
	void load(Space_display &space, int windowWidth, int windowHeight, std::vector<Player> &players, Map &map);
	void unload();
	void rotate();
	void animateShips();
	void changeSpeed();
	void moveShips();
	void setShipsPositions();
	void collectSpaceJunk();
	bool nearSpaceJunk(int index);
	void moveFireballs();
	void makeFireballs();
	void checkGameOver();
	void update();
	void fire();
	GameMode dock();
	bool nearPlanet(int index);
	bool nearSpaceJunk(void);
	void collide();
	void hyperDrive();
	void hyperAlignAngle();
	void hyperMove();
	void hyperEnterDigit();
	void hyperBackDigit();
	void hyperTextEntered();
	void abortHyperDrive();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	void keyRelease(ALLEGRO_EVENT &keyReleased);
	void triggerAnimation1();
	void triggerAnimation2();
	void checkAnimations();
	void AIUpdate();
	~Space_logic(void);
};

#endif