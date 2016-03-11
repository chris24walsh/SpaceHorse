#include "Menu_logic.h"


Menu_logic::Menu_logic(void)
{
}

void Menu_logic::load(Menu_display &menu)
{
	menu.load();
	m_menu = &menu;
}

void Menu_logic::unload()
{
	(*m_menu).unload();
}

int Menu_logic::keyPress(ALLEGRO_EVENT keyPressed)
{
	if (keyPressed.keyboard.keycode == ALLEGRO_KEY_UP) 
	{
		homeScreenOption--;
		if (homeScreenOption < 3) homeScreenOption = 4;
	}
	if (keyPressed.keyboard.keycode == ALLEGRO_KEY_DOWN)
	{
		homeScreenOption++;
		if (homeScreenOption > 4) homeScreenOption = 3;
	}
	if (keyPressed.keyboard.keycode == ALLEGRO_KEY_ENTER)
	{
		if (homeScreenOption==4) { return -1; } //Quit
		if (homeScreenOption==2) { return 3; } //Join server
		return 1; //Host on localhost or play single player
			//setUpHost();
	}
}

int Menu_logic::getHomeScreenOption() { return homeScreenOption; }

Menu_logic::~Menu_logic(void)
{
}
