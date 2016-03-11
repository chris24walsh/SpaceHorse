#ifndef LOGIC_H
#define LOGIC_H

#include "Game.h"
#include "Player.h"
#include "Menu_logic.h"
#include "Space_logic.h"
#include "Dock_logic.h"
//#include "Server_logic.h"

class Logic
{
private:
	Display *m_display;
	std::vector<Player> *m_players;
	Map *m_map;
	Menu_logic menu;
	Space_logic space;
	Dock_logic dock;
//	Server_logic server;
	int m_screenMode;
	std::string m_ipAddress;
	bool m_gameOver;
	bool m_hyperDrive;
	bool m_done;

public:
	Logic(void);
	void load(Display &display, std::vector<Player> &players, Map &map);
	void update();
	void pressKey(ALLEGRO_EVENT keyPressed);
	void releaseKey(ALLEGRO_EVENT keyReleased);
	Menu_logic& getMenu();
	Dock_logic& getDock();
	void changeScreen(int oldScreenMode);
	int getScreenMode();
	bool getDone();
	bool getGameOver();
	bool getHyperDrive();
	std::string getIpAddress();
	~Logic(void);
};

#endif