#include "Load_game_logic.h"


Load_game_logic::Load_game_logic()
	:m_loadGame(),
	m_homeScreenOption(1) //'new game' option selected by default
{
}

void Load_game_logic::load(Load_game_display &loadGame)
{
	loadGame.load();
	m_loadGame = &loadGame;
	m_numScreenOptions = (int)((*m_loadGame).getNumberSaveGames());
}

void Load_game_logic::unload()
{
	(*m_loadGame).unload();
}

void Load_game_logic::update() {}

GameMode Load_game_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		{
			m_homeScreenOption--;
			if (m_homeScreenOption < 1) { m_homeScreenOption = (*m_loadGame).getNumberSaveGames(); }
		}
		break;
	case ALLEGRO_KEY_DOWN:
		{
			m_homeScreenOption++;
			if (m_homeScreenOption >(*m_loadGame).getNumberSaveGames()) { m_homeScreenOption = 1; }
		}
		break;
	case ALLEGRO_KEY_ENTER:
		{
			if (m_homeScreenOption != m_numScreenOptions) //Load selected game
			{
				return GameMode::space;
			}
			if (m_homeScreenOption == m_numScreenOptions) //Go back to menu
			{
				return GameMode::menu;
			}

		}
	}
	(*m_loadGame).setHomeScreenOption(m_homeScreenOption); //tell display which option to light up
	return GameMode::loadGame; //otherwise stay in loadGame
}

Load_game_logic::~Load_game_logic(void)
{
}
