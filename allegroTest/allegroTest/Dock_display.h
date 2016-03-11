#ifndef DOCK_DISPLAY_H
#define DOCK_DISPLAY_H

#include "Game.h"

class Dock_display
{
private:
	bool m_dockFail;
	bool m_dockLoaded;

public:
	Dock_display(void);
	void load();
	void unload();
	void update(bool dispUpgradeText);
	bool getDockLoaded();
	bool getDockFail();
	~Dock_display(void);
};

#endif