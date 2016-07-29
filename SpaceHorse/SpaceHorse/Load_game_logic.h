#ifndef LOAD_GAME_LOGIC_H
#define LOAD_GAME_LOGIC_H

#include "Header.h"

#include "Load_game_display.h"
#include "Game_mode.h"

class Load_game_logic
{
private:
	Load_game_display *m_loadGame;
	int m_homeScreenOption;
	int m_numScreenOptions;

public:
	Load_game_logic();
	void load(Load_game_display &loadGame);
	void unload();
	void update();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	~Load_game_logic(void);
};

#endif