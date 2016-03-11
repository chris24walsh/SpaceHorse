#ifndef DOCK_LOGIC_H
#define DOCK_LOGIC_H

#include "Game.h"
#include "Dock_display.h"

class Dock_logic
{
private:
	Dock_display *m_dock;
	bool m_dispUpgradeText;

public:
	Dock_logic(void);
	void load(Dock_display &dock);
	void unload();
	void update();
	void upgrade_weapon(Player &player);
	int keyPress(ALLEGRO_EVENT keyPressed, Player &player);
	void keyRelease(ALLEGRO_EVENT keyReleased);
	bool getDispUpgradeText();
	~Dock_logic(void);
};

#endif