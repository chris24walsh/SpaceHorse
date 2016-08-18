#ifndef LOGIC_H
#define LOGIC_H

#include "Header.h"

#include "Display.h"
#include "Player.h"
#include "AI.h"
#include "Map.h"
#include "Menu_logic.h"
#include "New_game_logic.h"
#include "Load_game_logic.h"
#include "Save_game_logic.h"
#include "Space_logic.h"
#include "Dock_logic.h"
#include "Game_mode.h"

class Logic
{
private:
	std::vector<Player> m_players;
	std::vector<AI> m_ais;
	Map m_map;
	Menu_logic m_menu_logic;
	New_game_logic m_new_game_logic;
	Load_game_logic m_load_game_logic;
	Save_game_logic m_save_game_logic;
	Space_logic m_space_logic;
	Dock_logic m_dock_logic;
	GameMode m_gameMode;
	bool m_done;
	bool m_gameStarted;

public:
	Logic();
	void load(Display &display);
	void update();
	void pressKey(ALLEGRO_EVENT &keyPressed, Display &display);
	void releaseKey(ALLEGRO_EVENT &keyReleased);
	void changeScreen(GameMode oldScreenMode, Display &display);
	bool getDone();
	~Logic(void);
};

#endif