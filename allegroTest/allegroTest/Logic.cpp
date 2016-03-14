#include "Logic.h"


Logic::Logic()
	:m_players(),
	m_map(),
	menu(),
	space(),
	dock(),
	m_screenMode(0),
	m_done(0)
{
}

void Logic::load(Display &display, std::vector<Player> &players, Map &map)
{
	m_players = &players;
	m_map = &map;
	menu.load(display.getMenu());
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

void Logic::pressKey(ALLEGRO_EVENT &keyPressed, Display &display)
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
		screenMode = dock.keyPress(keyPressed);
		break;
//	case 3: // Server
//		server.keyPress(keyPressed);
//		break;
	}
	if(screenMode==-1) { m_done = true; } //Quit

	else if(screenMode != m_screenMode) //if screenmode is changed, load new screen
	{ 
		int oldScreenMode = m_screenMode;
		m_screenMode = screenMode;
		changeScreen(oldScreenMode, display);
	}
}

void Logic::releaseKey(ALLEGRO_EVENT &keyReleased)
{
	switch(m_screenMode)
	{
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

void Logic::changeScreen(int oldScreenMode, Display &display)
{
	display.setScreenMode(m_screenMode);
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
	case 0: //passes the display to logic (for future manipulation/control) and tells logic to tell display to load resources (e.g. bitmaps, fonts, etc)
		menu.load(display.getMenu());
		break;
	case 1:
		space.load(display.getSpace(), display.getWindowWidth(), display.getWindowHeight(), *m_players, *m_map);
		break;
	case 2:
		dock.load(display.getDock(), (*m_players).at(0));
		break;
//	case 3:
//		server;
//		break;
	}
}

Menu_logic& Logic::getMenu() { return menu; }
Space_logic& Logic::getSpace() { return space; }
Dock_logic& Logic::getDock() { return dock; }
int Logic::getScreenMode() { return m_screenMode; }
bool Logic::getDone() { return m_done; }

Logic::~Logic(void)
{
}
