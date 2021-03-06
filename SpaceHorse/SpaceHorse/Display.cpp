#include "Display.h"

Display::Display(int width, int height)
	:menu(width, height),
	newGame(width, height),
	loadGame(width, height),
	saveGame(width, height),
	space(width, height),
	dock(width, height),
	m_windowWidth(width),
	m_windowHeight(height),
	m_gameMode(GameMode::menu)
{
}

void Display::update()
	{
	switch (m_gameMode)
	{
	case GameMode::menu:
		menu.update();
		break;
	case GameMode::newGame:
		newGame.update();
		break;
	case GameMode::loadGame:
		loadGame.update();
		break;
	case GameMode::saveGame:
		saveGame.update();
		break;
	case GameMode::space:
		space.update();
		break;
	case GameMode::dock:
		dock.update();
		break;

	}
}

Menu_display& Display::getMenu() { return menu; }
New_game_display& Display::getNewGame() { return newGame; }
Load_game_display& Display::getLoadGame() { return loadGame; }
Save_game_display& Display::getSaveGame() { return saveGame; }
Space_display& Display::getSpace() { return space; }
Dock_display& Display::getDock() { return dock; }
int Display::getWindowWidth() { return m_windowWidth; }
int Display::getWindowHeight() { return m_windowHeight; }
void Display::setScreenMode(GameMode screenMode) { m_gameMode = screenMode; }

Display::~Display(void)
{
}
