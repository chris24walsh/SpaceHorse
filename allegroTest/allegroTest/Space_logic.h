#ifndef SPACE_LOGIC_H
#define SPACE_LOGIC_H

#include "Game.h"
#include "Space_display.h"

class Space_logic
{
private:
	Space_display *m_space;

public:
	Space_logic(void);
	void load(Space_display &space);
	void unload();
	void update();
	int dock();
	int keyPress(ALLEGRO_EVENT keyPressed);
	void keyRelease(ALLEGRO_EVENT keyReleased);
	~Space_logic(void);
};

#endif