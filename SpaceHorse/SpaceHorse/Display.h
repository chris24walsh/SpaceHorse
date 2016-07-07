#ifndef DISPLAY_H
#define DISPLAY_H

#include "Header.h"

#include "Menu_display.h"
#include "Space_display.h"
#include "Dock_display.h"

class Display
{
private:
	Menu_display menu;
	Space_display space;
	Dock_display dock;
	int m_windowWidth,
		m_windowHeight,
		m_screenMode;

public:
	Display(int width, int height);
	void update();
	Menu_display& getMenu();
	Space_display& getSpace();
	Dock_display& getDock();
	int getWindowWidth();
	int getWindowHeight();
	void setScreenMode(int screenMode);
	~Display(void);
};

#endif