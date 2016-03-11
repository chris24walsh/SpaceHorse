#include "Dock_logic.h"


Dock_logic::Dock_logic(void)
{
}

void Dock_logic::load(Dock_display &dock)
{
	dock.load();
	m_dock = &dock;
}

void Dock_logic::unload()
{
	(*m_dock).unload();
}

void Dock_logic::update() {} //put any dock logic updating here

void Dock_logic::upgrade_weapon(Player &player)
{
	m_dispUpgradeText = true;
	for (int i=0; i<static_cast<int>(Fire::MAXFIREBALLS); i++) player.getShip().getFireSprite[i] = al_load_bitmap("c:/dev/allegro/images/fireball2.png");
	players.at(0).fireHeight = 40;
	
}

int Dock_logic::keyPress(ALLEGRO_EVENT keyPressed, Player &player)
{
			if (keyPressed.keyboard.keycode == ALLEGRO_KEY_W) { upgrade_weapon(player); }
			if (keyPressed.keyboard.keycode == ALLEGRO_KEY_D) { return 1; } //switch back to space screen
			return 2; //i.e. screenmode will not change
}

bool Dock_logic::getDispUpgradeText() { return m_dispUpgradeText; }

Dock_logic::~Dock_logic(void)
{
}
