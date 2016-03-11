#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_H

#include "Game.h"

class Menu_display
{
private:
	ALLEGRO_FONT *font;
	bool m_menuFail;

public:
	Menu_display(void);
	void load();
	void unload();
	void update(int homeScreenOption, int windowWidth, int windowHeight);
	void menuFail(std::string failMessage);
	~Menu_display(void);
};

#endif