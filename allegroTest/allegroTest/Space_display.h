#ifndef SPACE_DISPLAY_H
#define SPACE_DISPLAY_H

#include "Game.h"

class Space_display
{
private:
	bool m_spaceFail;
	bool m_spaceLoaded;

public:
	Space_display(void);
	void load();
	void unload();
	void update(Logic &logic, std::vector<Player> &players, Map &map);
	bool getSpaceLoaded();
	bool getSpaceFail();
	~Space_display(void);
};

#endif