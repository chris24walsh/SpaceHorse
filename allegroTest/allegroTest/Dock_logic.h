#ifndef DOCK_LOGIC_H
#define DOCK_LOGIC_H

#include "Header.h"

#include "Dock_display.h"
#include "Player.h"

class Dock_logic
{
private:
	Dock_display *m_dock;
	Player *m_player;

public:
	Dock_logic();
	void load(Dock_display &dock, Player &player);
	void unload();
	void update();
	void upgrade_weapon();
	int keyPress(ALLEGRO_EVENT &keyPressed);
	~Dock_logic(void);
};

#endif