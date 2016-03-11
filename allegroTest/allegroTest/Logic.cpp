#include "Logic.h"


Logic::Logic(void)
{
}

void Logic::load(Display &display, std::vector<Player> &players, Map &map)
{
	m_display = &display;
	m_players = &players;
	m_map = &map;
	changeScreen(); //not a case for oldScreenMode because we are initialising
}

void Logic::update() 
{
	switch(m_screenMode) 
	{
	case 0: // Menu
		menu.update();
		break;
	case 1: // Space
		space.update();
		break;
	case 2: // Dock
		dock.update();
		break;
//	case 3: // Server
//		server.update();
//		break;
	}
}

void Logic::pressKey(ALLEGRO_EVENT keyPressed) 
{
	int screenMode;
	switch(m_screenMode) 
	{
	case 0: // Menu
		screenMode = menu.keyPress(keyPressed);
		break;
	case 1: // Space
		screenMode = space.keyPress(keyPressed);
		break;
	case 2: // Dock
		screenMode = dock.keyPress(keyPressed, (*m_players).at(0));
		break;
//	case 3: // Server
//		server.keyPress(keyPressed);
//		break;
	}
	if(screenMode==-1) { m_done = true; } //Quit

	if(screenMode /= m_screenMode) //if screenmode is changed, load new screen
	{ 
		int oldScreenMode = m_screenMode;
		m_screenMode = screenMode;
		changeScreen(oldScreenMode);
	}
}

void Logic::releaseKey(ALLEGRO_EVENT keyReleased)
{
	switch(m_screenMode) {
	case 0: //Menu
//		m_screenMode = menu.keyRelease(keyReleased);
		break;
	case 1: //Space mode
		space.keyRelease(keyReleased);
		break;
	case 2: //Dock
//		m_screenMode = dock.keyRelease(keyReleased);
		break;
//	case 3: //Server
//		m_screenMode = server.keyRelease(keyReleased);
//		break;	
	}
}

void Logic::changeScreen(int oldScreenMode=-1) // -1 for default
{
	switch(oldScreenMode)
	{
	case 0:
		menu.unload(); //tells logic to tell old display to unload resources (e.g. bitmaps, etc)
		break;
	case 1:
		space.unload();
		break;
	case 2:
		dock.unload();
		break;
//	case 3:
//		server;
//		break;
	}
	switch(m_screenMode)
	{
	case 0:
		menu.load((*m_display).getMenu); //passes the display to logic (for future manipulation/control) and tells logic to tell display to load resources (e.g. bitmaps, fonts, etc)
		break;
	case 1:
		space.load((*m_display).getSpace);
		break;
	case 2:
		dock.load((*m_display).getDock);
		break;
//	case 3:
//		server;
//		break;
	}
}

Menu_logic& Logic::getMenu() { return menu; }
Dock_logic& Logic::getDock() { return dock; }
int Logic::getScreenMode() { return m_screenMode; }

Logic::~Logic(void)
{
}
