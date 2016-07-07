#include "Display.h"

Display::Display(int width, int height)
	:menu(width, height),
	space(width, height),
	dock(width, height),
	m_windowWidth(width),
	m_windowHeight(height),
	m_screenMode(0)
{
}

void Display::update()
	{
	switch (m_screenMode)
	{
	//Home screen mode
	case 0:
		menu.update();
		break;
	//Space mode
	case 1:
		space.update();
		break;

	//Docking mode
	case 2:
		dock.update();
		break;

	//Join server mode
//	case 3:
//		break;
	}
}

Menu_display& Display::getMenu() { return menu; }
Space_display& Display::getSpace() { return space; }
Dock_display& Display::getDock() { return dock; }
int Display::getWindowWidth() { return m_windowWidth; }
int Display::getWindowHeight() { return m_windowHeight; }
void Display::setScreenMode(int screenMode) { m_screenMode = screenMode; }

Display::~Display(void)
{
}
