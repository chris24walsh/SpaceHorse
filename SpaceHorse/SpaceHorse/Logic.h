#ifndef LOGIC_H
#define LOGIC_H

#include "Header.h"

#include "Display.h"
#include "Player.h"
#include "Menu_logic.h"
#include "Space_logic.h"
#include "Dock_logic.h"
#include "Game_mode.h"

class Logic
{
private:
	std::vector<Player> *m_players;
	Map *m_map;
	Menu_logic m_menu_logic;
	Space_logic m_space_logic;
	Dock_logic m_dock_logic;
	GameMode m_gameMode;
	bool m_done;

public:
	Logic();
	void load(Display &display, std::vector<Player> &players, Map &map);
	void update();
	void pressKey(ALLEGRO_EVENT &keyPressed, Display &display);
	void releaseKey(ALLEGRO_EVENT &keyReleased);
	void changeScreen(GameMode oldScreenMode, Display &display);
	bool getDone();
	~Logic(void);
};

#endif