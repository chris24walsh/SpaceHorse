#include "Dock_logic.h"


Dock_logic::Dock_logic()
	:m_player(),
	m_dock()
{
}

void Dock_logic::load(Dock_display &dock, Player &player, Planet &planet)
{
	dock.load(player, planet);
	m_dock = &dock;
	m_player = &player;
	m_planet = &planet;
}

void Dock_logic::unload()
{
	(*m_dock).unload();
}

void Dock_logic::update() {} //put any dock logic updating here

void Dock_logic::upgrade_weapon()
{
	if((*m_planet).getCanUpgrade()) //check if possible at this planet
	{
		(*m_player).getShip().setUpgraded("c:/dev/allegro/images/fireball2.png"); //set new file location for image
		(*m_dock).setDispUpgradeText(); //load new fireball image
		(*m_player).getShip().setFireSize((*m_dock).getFireSpriteWidth(), (*m_dock).getFireSpriteHeight()); //set new firing parameters
	}
	else { (*m_dock).setFailedUpgrade(); }
}

int Dock_logic::keyPress(ALLEGRO_EVENT &keyPressed)
{
	switch(keyPressed.keyboard.keycode)
	{
	case (ALLEGRO_KEY_W):
		upgrade_weapon();
		break;
	case ALLEGRO_KEY_D:
		(*m_player).getShip().toggleDocked();
		return 1; //switch back to space screen
	case ALLEGRO_KEY_ESCAPE:
		return -1;
	}
	return 2; //i.e. screenmode will not change
}

Dock_logic::~Dock_logic(void)
{
}
