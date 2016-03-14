#include "Menu_logic.h"


Menu_logic::Menu_logic()
	:m_menu(),
	m_homeScreenOption(3) //single player option lit up by default
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
			if (m_homeScreenOption < 3) { m_homeScreenOption = 4; }
		}
		break;
	case ALLEGRO_KEY_DOWN:
		{
			m_homeScreenOption++;
			if (m_homeScreenOption > 4) { m_homeScreenOption = 3; }
		}
		break;
	case ALLEGRO_KEY_ENTER:
		{
			switch(m_homeScreenOption)
			{
			case 2:
				return 3; //Join server
			case 3:
				return 1; //Host on localhost or play single player
			case 4:
				return -1; //Quit
			//setUpHost();
			}
		}
	}
	(*m_menu).setHomeScreenOption(m_homeScreenOption); //tell display which option to light up
	return 0; //otherwise stay in menu
}

Menu_logic::~Menu_logic(void)
{
}
