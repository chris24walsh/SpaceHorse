#ifndef MENU_LOGIC_H
#define MENU_LOGIC_H

#include "Header.h"

#include "Menu_display.h"

class Menu_logic
{
private:
	Menu_display *m_menu;
	int m_homeScreenOption;

public:
	Menu_logic();
	void load(Menu_display &menu);
	void unload();
	void update();
	int keyPress(ALLEGRO_EVENT &keyPressed);
	~Menu_logic(void);
};

#endif