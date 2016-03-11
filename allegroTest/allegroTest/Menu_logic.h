#ifndef MENU_LOGIC_H
#define MENU_LOGIC_H

#include "Game.h"
#include "Menu_display.h"

class Menu_logic
{
private:
	Menu_display *m_menu;
	int homeScreenOption;

public:
	Menu_logic(void);
	void load(Menu_display &menu);
	void unload();
	void update();
	int keyPress(ALLEGRO_EVENT keyPressed);
	void keyRelease(ALLEGRO_EVENT keyReleased);
	int getHomeScreenOption();
	bool getDone();
	~Menu_logic(void);
};

#endif