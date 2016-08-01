#ifndef SAVE_GAME_LOGIC_H
#define SAVE_GAME_LOGIC_H

#include "Header.h"

#include "Save_game_display.h"
#include "Game_mode.h"
#include "Player.h"

class Save_game_logic
{
private:
	Save_game_display *m_saveGame;
	int m_homeScreenOption;
	int m_numScreenOptions;
	std::vector<Player> *m_players;

public:
	Save_game_logic();
	void load(Save_game_display &saveGame, std::vector<Player> &players);
	void unload();
	void update();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	~Save_game_logic(void);
};

#endif