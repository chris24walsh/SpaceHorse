#include "Dock_logic.h"


Dock_logic::Dock_logic()
	:m_player(),
	m_dock()
{
}

void Dock_logic::load(Dock_display &dock, Player &player)
{
	dock.load(player);
	m_dock = &dock;
	m_player = &player;
}

void Dock_logic::unload()
{
	(*m_dock).unload();
}

void Dock_logic::update() {} //put any dock logic updating here

void Dock_logic::upgrade_weapon()
{
	(*m_player).getShip().setUpgraded("c:/dev/allegro/images/fireball2.png");
	(*m_dock).setDispUpgradeText();
	(*m_player).getShip().setFireSize((*m_dock).getFireSpriteWidth(), (*m_dock).getFireSpriteHeight());
}

int Dock_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
			if (keyPressed.keyboard.keycode == ALLEGRO_KEY_W) { upgrade_weapon(); }
			if (keyPressed.keyboard.keycode == ALLEGRO_KEY_D) { (*m_player).getShip().toggleDocked(); return 1; } //switch back to space screen
			return 2; //i.e. screenmode will not change
}

Dock_logic::~Dock_logic(void)
{
}
