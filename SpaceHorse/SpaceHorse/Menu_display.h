#ifndef MENU_DISPLAY_H
#define MENU_DISPLAY_H

#include "Header.h"

class Menu_display
{
private:
	bool m_menuLoad;
	int m_windowWidth,
		m_windowHeight,
		m_homeScreenOption;
	ALLEGRO_FONT *m_font;
	ALLEGRO_SAMPLE *m_music;

public:
	Menu_display(int windowWidth, int windowHeight);
	void load();
	void unload();
	void update();
	void setHomeScreenOption(int homeScreenOption);
	void menuFail(std::string failMessage);
	~Menu_display(void);
};

#endif