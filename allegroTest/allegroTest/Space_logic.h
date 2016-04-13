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
		m_textEntered;
	int m_newKey,
		m_distance,
		m_maxX,
		m_maxY;
	std::string m_editText;

public:
	Space_logic();
	void load(Space_display &space, int windowWidth, int windowHeight, std::vector<Player> &players, Map &map);
	void unload();
	void update();
	int keyPress(ALLEGRO_EVENT &keyPressed);
	void keyRelease(ALLEGRO_EVENT &keyReleased);

	void animateShips();
	void changeVelocity();
	void moveShips();
	
	void fire();
	void moveFireballs();
	void makeFireballs();

	int dock();
	bool nearPlanet(int index);
	void collide();
	void checkGameOver();

	void hyperDrive();
	void hyperEnterDigit();
	void hyperBackDigit();
	void hyperTextEntered();
	void abortHyperDrive();

	~Space_logic(void);
};

#endif