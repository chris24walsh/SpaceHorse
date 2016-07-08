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

GameMode Menu_logic::keyPress(ALLEGRO_EVENT &keyPressed)
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
				return GameMode::space; //Start new game (go to space screen)
			case 2:
				return GameMode::space; //Load existing game (go to space screen)
			case 3:
				return GameMode::quit; //Quit
			}
		}
	}
	(*m_menu).setHomeScreenOption(m_homeScreenOption); //tell display which option to light up
	return GameMode::menu; //otherwise stay in menu
}

Menu_logic::~Menu_logic(void)
{
}
