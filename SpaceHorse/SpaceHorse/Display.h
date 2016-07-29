#ifndef DISPLAY_H
#define DISPLAY_H

#include "Header.h"

#include "Menu_display.h"
#include "New_game_display.h"
#include "Load_game_display.h"
#include "Space_display.h"
#include "Dock_display.h"
#include "Game_mode.h"

class Display
{
private:
	Menu_display menu;
	New_game_display newGame;
	Load_game_display loadGame;
	Space_display space;
	Dock_display dock;
	int m_windowWidth;
	int m_windowHeight;
	GameMode m_gameMode;

public:
	Display(int width, int height);
	void update();
	Menu_display& getMenu();
	New_game_display& getNewGame();
	Load_game_display& getLoadGame();
	Space_display& getSpace();
	Dock_display& getDock();
	int getWindowWidth();
	int getWindowHeight();
	void setScreenMode(GameMode screenMode);
	~Display(void);
};

#endif