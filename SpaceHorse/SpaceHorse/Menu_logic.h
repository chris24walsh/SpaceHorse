#ifndef MENU_LOGIC_H
#define MENU_LOGIC_H

#include "Header.h"

#include "Menu_display.h"
#include "Game_mode.h"

class Menu_logic
{
private:
	Menu_display *m_menu;
	int m_homeScreenOption;
	bool *m_gameStarted;

public:
	Menu_logic();
	void load(Menu_display &menu, bool &gameStarted);
	void unload();
	void update();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	~Menu_logic(void);
};

#endif