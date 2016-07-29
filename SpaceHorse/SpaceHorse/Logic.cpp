#include "Logic.h"


Logic::Logic()
	:m_players(1, Player()),
	m_map(),
	m_menu_logic(),
	m_new_game_logic(),
	m_load_game_logic(),
	m_space_logic(),
	m_dock_logic(),
	m_gameMode(GameMode::menu),
	m_done(0)
{
	//load map
	m_map.makeSolarSystem();
}

void Logic::load(Display &display)
{
	m_menu_logic.load(display.getMenu());
}

void Logic::update() 
{
	switch(m_gameMode) 
	{
	case GameMode::menu:
		m_menu_logic.update();
		break;
	case GameMode::newGame:
		m_new_game_logic.update();
		break;
	case GameMode::loadGame:
		m_load_game_logic.update();
		break;
	case GameMode::space:
		m_space_logic.update();
		break;
	case GameMode::dock:
		m_dock_logic.update();
		break;
	}
}

void Logic::pressKey(ALLEGRO_EVENT &keyPressed, Display &display)
{
	GameMode gameMode;
	switch(m_gameMode) 
	{
	case GameMode::menu:
		gameMode = m_menu_logic.keyPress(keyPressed);
		break;
	case GameMode::newGame:
		gameMode = m_new_game_logic.keyPress(keyPressed);
		break;
	case GameMode::loadGame:
		gameMode = m_load_game_logic.keyPress(keyPressed);
		break;
	case GameMode::space:
		gameMode = m_space_logic.keyPress(keyPressed);
		break;
	case GameMode::dock:
		gameMode = m_dock_logic.keyPress(keyPressed);
		break;
	}
	if(gameMode == GameMode::quit) { m_done = true; } //Quit

	else if(gameMode != m_gameMode) //if screenmode is changed, load new screen
	{ 
		GameMode oldScreenMode = m_gameMode;
		m_gameMode = gameMode;
		changeScreen(oldScreenMode, display);
	}
}

void Logic::releaseKey(ALLEGRO_EVENT &keyReleased)
{
	switch(m_gameMode)
	{
	case GameMode::menu:
//		m_screenMode = menu.keyRelease(keyReleased);
		break;
	case GameMode::newGame:
		//		m_screenMode = newGame.keyRelease(keyReleased);
		break;
	case GameMode::loadGame:
		//		m_screenMode = loadGame.keyRelease(keyReleased);
		break;
	case GameMode::space:
		m_space_logic.keyRelease(keyReleased);
		break;
	case GameMode::dock:
//		m_screenMode = dock.keyRelease(keyReleased);
		break;
	}
}

void Logic::changeScreen(GameMode oldScreenMode, Display &display)
{
	display.setScreenMode(m_gameMode);
	switch(oldScreenMode)
	{
	case GameMode::menu:
		m_menu_logic.unload(); //tells logic to tell old display to unload resources (e.g. bitmaps, etc)
		break;
	case GameMode::newGame:
		m_new_game_logic.unload();
		break;
	case GameMode::loadGame:
		m_load_game_logic.unload();
		break;
	case GameMode::space:
		m_space_logic.unload();
		break;
	case GameMode::dock:
		m_dock_logic.unload();
		break;
	}

	switch(m_gameMode)
	{
	case GameMode::menu: //passes the display to logic (for future manipulation/control) and tells logic to tell display to load resources (e.g. bitmaps, fonts, etc)
		m_menu_logic.load(display.getMenu());
		break;
	case GameMode::newGame:
		m_new_game_logic.load(display.getNewGame()); //tells logic to tell old display to unload resources (e.g. bitmaps, etc)
		break;
	case GameMode::loadGame:
		m_load_game_logic.load(display.getLoadGame()); //tells logic to tell old display to unload resources (e.g. bitmaps, etc)
		break;
	case GameMode::space:
		m_space_logic.load(display.getSpace(), display.getWindowWidth(), display.getWindowHeight(), m_players, m_map);
		break;
	case GameMode::dock:
		m_dock_logic.load(display.getDock(), (m_players).at(0), (m_map).getPlanets().at((m_players).at(0).getShip().getDockPlanet()));
		break;
	}
}

bool Logic::getDone() { return m_done; }

Logic::~Logic(void)
{
}
