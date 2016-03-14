#include "Dock_display.h"


Dock_display::Dock_display(int windowWidth, int windowHeight)
	:m_dispUpgradeText(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_shipSprite(nullptr),
	m_fireSprite(nullptr),
	m_dockingText(nullptr),
	m_upgradedText(nullptr),
	m_player(),
	m_dockLoad(0)
{
}

void Dock_display::load(Player &player)
{
	if(!m_dockLoad)
	{
		m_player = &player;
		m_dockingText = al_load_bitmap("c:/dev/allegro/images/dockingText.png");
		if(!m_dockingText) { dockFail("Failed to load the dockingText image"); }
		if(player.getShip().getUpgraded())
		{
			m_upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
			if(!m_upgradedText) { dockFail("Failed to load the upgradedText image"); }
		}

		m_shipSprite = al_load_bitmap(player.getShip().getShipSprite().c_str()); //converting to c-style string because allegro is archaic or something...
		if(!m_shipSprite) { dockFail("Failed to load the shipSprite image"); }
		m_fireSprite = al_load_bitmap(player.getShip().getFireSprite().c_str());
		if(!m_fireSprite) { dockFail("Failed to load the fireball image"); }

		m_dockLoad = true;
	}
}

void Dock_display::unload()
{
	if(m_dockLoad)
	{
		if (m_dockingText) { al_destroy_bitmap(m_dockingText); }
		if (m_upgradedText) { al_destroy_bitmap(m_upgradedText); }
		if (m_shipSprite) { al_destroy_bitmap(m_shipSprite); }
		if (m_fireSprite) { al_destroy_bitmap(m_fireSprite); }
		m_dockLoad = false;
	}
}

void Dock_display::update()
{
	al_clear_to_color(al_map_rgb(25,0,25));
	al_draw_rotated_bitmap(m_shipSprite, al_get_bitmap_width(m_shipSprite)/2, al_get_bitmap_height(m_shipSprite)/2, 250, 250, 0, 0);
	al_draw_rotated_bitmap(m_fireSprite, al_get_bitmap_width(m_fireSprite)/2, al_get_bitmap_height(m_fireSprite)/2, 250, 250, 0, 0);
	al_draw_bitmap(m_dockingText, 0, 0, 0);
	if ((*m_player).getShip().getUpgraded()) al_draw_bitmap(m_upgradedText, 0, m_windowHeight/2, 0);
}

void Dock_display::setDispUpgradeText()
{
	m_upgradedText = al_load_bitmap("c:/dev/allegro/images/upgradedText.png");
	if(!m_upgradedText) { dockFail("Failed to load the upgradedText image"); }

	if (m_fireSprite) al_destroy_bitmap(m_fireSprite);
	m_fireSprite = al_load_bitmap((*m_player).getShip().getFireSprite().c_str());
	if(!m_fireSprite) { dockFail("Failed to load the fireball image"); }
}

int Dock_display::getFireSpriteWidth() { return al_get_bitmap_width(m_fireSprite); }
int Dock_display::getFireSpriteHeight() { return al_get_bitmap_height(m_fireSprite); }

void Dock_display::dockFail(std::string failMessage)
{
    printf("%s \n", failMessage);
	_getch();
    exit(1);
}

Dock_display::~Dock_display(void)
{
	if(m_dockLoad)
	{
		if (m_dockingText) { al_destroy_bitmap(m_dockingText); }
		if (m_upgradedText) { al_destroy_bitmap(m_upgradedText); }
		if (m_shipSprite) { al_destroy_bitmap(m_shipSprite); }
		if (m_fireSprite) { al_destroy_bitmap(m_fireSprite); }
		m_dockLoad = false;
	}
}
