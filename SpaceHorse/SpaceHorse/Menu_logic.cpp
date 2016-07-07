#include "Menu_logic.h"


Menu_logic::Menu_logic()
	:m_menu(),
	m_homeScreenOption(1) //'new game' option selected by default
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

void Menu_logic::update() {}

int Menu_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		{
			m_homeScreenOption--;
			if (m_homeScreenOption < 1) { m_homeScreenOption = 3; }
		}
		break;
	case ALLEGRO_KEY_DOWN:
		{
			m_homeScreenOption++;
			if (m_homeScreenOption > 3) { m_homeScreenOption = 1; }
		}
		break;
	case ALLEGRO_KEY_ENTER:
		{
			switch(m_homeScreenOption)
			{
			case 1:
				return 1; //Start new game (go to space screen)
			case 2:
				return 1; //Load existing game (go to space screen)
			case 3:
				return -1; //Quit
			}
		}
	/*case ALLEGRO_KEY_ESCAPE:
		return -1;*/
	}
	(*m_menu).setHomeScreenOption(m_homeScreenOption); //tell display which option to light up
	return 0; //otherwise stay in menu
}

Menu_logic::~Menu_logic(void)
{
}
