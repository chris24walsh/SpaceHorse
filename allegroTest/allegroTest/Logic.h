#ifndef LOGIC_H
#define LOGIC_H

#include "Header.h"

#include "Display.h"
#include "Player.h"
#include "Menu_logic.h"
#include "Space_logic.h"
#include "Dock_logic.h"
//#include "Server_logic.h"

class Logic
{
private:
	std::vector<Player> *m_players;
	Map *m_map;
	Menu_logic menu;
	Space_logic space;
	Dock_logic dock;
//	Server_logic server;
	int m_screenMode;
	bool m_done;

public:
	Logic();
	void load(Display &display, std::vector<Player> &players, Map &map);
	void update();
	void pressKey(ALLEGRO_EVENT &keyPressed, Display &display);
	void releaseKey(ALLEGRO_EVENT &keyReleased);
	Menu_logic& getMenu();
	Space_logic& getSpace();
	Dock_logic& getDock();
	void changeScreen(int oldScreenMode, Display &display);
	int getScreenMode();
	bool getDone();
	~Logic(void);
};

#endif