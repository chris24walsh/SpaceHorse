#include "Dock_display.h"


Dock_display::Dock_display(int windowWidth, int windowHeight)
	:m_failedUpgrade(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_fireSprite(nullptr),
	m_dockingText(nullptr),
	m_upgradedText(nullptr),
	m_font(nullptr),
	m_player(),
	m_dockLoad(0)
{
}

void Dock_display::load(Player &player, Planet &planet)
{
	if(!m_dockLoad)
	{
		m_player = &player;
		m_planet = &planet;

		m_font = al_load_ttf_font("../../Font/pirulen.ttf",25,0);
		if(!m_font) { dockFail("Could not load 'pirulen.ttf'.\n"); }

		m_dockingText = al_load_bitmap("../../images/dockingText.png");
		if(!m_dockingText) { dockFail("Failed to load the dockingText image"); }
		if(player.getShip().getUpgraded())
		{
			m_upgradedText = al_load_bitmap("../../images/upgradedText.png");
			if(!m_upgradedText) { dockFail("Failed to load the upgradedText image"); }
		}

		m_fireSprite = al_load_bitmap(player.getShip().getFireSprite().c_str());
		if(!m_fireSprite) { dockFail("Failed to load the fireball image"); }

		m_dockLoad = true;
	}
}

void Dock_display::unload()
{
	if(m_dockLoad)
	{
		if (m_dockingText) { al_destroy_bitmap(m_dockingText); m_dockingText=nullptr; }
		if (m_upgradedText) { al_destroy_bitmap(m_upgradedText); m_upgradedText=nullptr; }
		if (m_fireSprite) { al_destroy_bitmap(m_fireSprite); m_fireSprite=nullptr; }
		if(m_font) { al_destroy_font(m_font); m_font=nullptr; }
		m_failedUpgrade = false;
		m_dockLoad = false;
	}
}

void Dock_display::update()
{
	if(!m_dockLoad) { load(*m_player, *m_planet); }
	al_clear_to_color(al_map_rgb(25,0,25));
	al_draw_rotated_bitmap(m_player->getShip().getShipSprite(),
		m_player->getShip().getWidth()/2, m_player->getShip().getHeight()/2,
		m_windowWidth*0.48, m_windowHeight*0.4, 0, 0);
	al_draw_rotated_bitmap(m_fireSprite, al_get_bitmap_width(m_fireSprite)/2, al_get_bitmap_height(m_fireSprite)/2, m_windowWidth*0.52, m_windowHeight*0.4, 0, 0);

//	al_draw_bitmap(m_dockingText, 0, 0, 0);
	std::stringstream ss_planetText;
	ss_planetText << "You have docked at " << m_planet->getPlanetName() << "."; //al_draw_text doesn't support newline (\n) so...
	std::string planetText = ss_planetText.str();
	al_draw_text(m_font, al_map_rgb(190,190,190), m_windowWidth*0.5, m_windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, planetText.c_str());

	std::string upgradeText;
	if((*m_player).getShip().getUpgraded())
	{
//		al_draw_bitmap(m_upgradedText, 0, m_windowHeight/2, 0);
		upgradeText = "Your weapon has been upgraded.";
	}
	else if(!m_failedUpgrade) { upgradeText = "To upgrade your weapon, press W."; }
	else { upgradeText = "Upgrading is impossible at this planet!"; }
	al_draw_text(m_font, al_map_rgb(190,190,190), m_windowWidth*0.5, m_windowHeight*0.3, ALLEGRO_ALIGN_CENTRE, upgradeText.c_str());
}

void Dock_display::setDispUpgradeText()
{
	m_upgradedText = al_load_bitmap("../../images/upgradedText.png");
	if(!m_upgradedText) { dockFail("Failed to load the upgradedText image"); }

	if (m_fireSprite) al_destroy_bitmap(m_fireSprite);
	m_fireSprite = al_load_bitmap((*m_player).getShip().getFireSprite().c_str());
	if(!m_fireSprite) { dockFail("Failed to load the fireball image"); }
}

void Dock_display::setFailedUpgrade() { m_failedUpgrade = true; }
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
		if (m_fireSprite) { al_destroy_bitmap(m_fireSprite); }
		m_dockLoad = false;
	}
}
