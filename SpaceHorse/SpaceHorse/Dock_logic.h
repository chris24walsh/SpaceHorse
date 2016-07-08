#ifndef DOCK_LOGIC_H
#define DOCK_LOGIC_H

#include "Header.h"

#include "Dock_display.h"
#include "Player.h"
#include "Planet.h"
#include "Game_mode.h"

class Dock_logic
{
private:
	Dock_display *m_dock;
	Player *m_player;
	Planet *m_planet;

public:
	Dock_logic();
	void load(Dock_display &dock, Player &player, Planet &planet);
	void unload();
	void update();
	void upgrade_weapon();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	~Dock_logic(void);
};

#endif