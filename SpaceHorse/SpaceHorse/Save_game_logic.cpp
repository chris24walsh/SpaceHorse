#include "Save_game_logic.h"
#include "Database.h"


Save_game_logic::Save_game_logic()
	:m_saveGame(),
	m_homeScreenOption(1) //'new game' option selected by default
{
}

void Save_game_logic::load(Save_game_display &saveGame, std::vector<Player> &players)
{
	saveGame.load();
	m_players = &players;
	m_saveGame = &saveGame;
	m_numScreenOptions = (int)((*m_saveGame).getNumberSaveGames());
}

void Save_game_logic::unload()
{
	(*m_saveGame).unload();
}

void Save_game_logic::update() {}

GameMode Save_game_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case ALLEGRO_KEY_UP:
		{
			m_homeScreenOption--;
			if (m_homeScreenOption < 1) { m_homeScreenOption = (*m_saveGame).getNumberSaveGames(); }
		}
		break;
	case ALLEGRO_KEY_DOWN:
		{
			m_homeScreenOption++;
			if (m_homeScreenOption >(*m_saveGame).getNumberSaveGames()) { m_homeScreenOption = 1; }
		}
		break;
	case ALLEGRO_KEY_ENTER:
		{
			if (m_homeScreenOption != m_numScreenOptions) //Save selected game
			{
				std::cout << "Saving game to " << (*m_saveGame).getSavedGames().at(m_homeScreenOption - 1) << std::endl;
				Database db((*m_saveGame).getSavedGames().at(m_homeScreenOption - 1)); //Create a database connection, passing in the name of the savefile chosen
				db.save(m_players->at(0));
				return GameMode::space;
			}
			if (m_homeScreenOption == m_numScreenOptions) //Go back to menu
			{
				return GameMode::menu;
			}

		}
	}
	(*m_saveGame).setHomeScreenOption(m_homeScreenOption); //tell display which option to light up
	return GameMode::saveGame; //otherwise stay in saveGame
}

Save_game_logic::~Save_game_logic(void)
{
}
