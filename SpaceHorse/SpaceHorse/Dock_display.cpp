#include "Dock_display.h"


Dock_display::Dock_display(int windowWidth, int windowHeight)
	:m_failedUpgrade(0),
	m_windowWidth(windowWidth),
	m_windowHeight(windowHeight),
	m_shipSprite(nullptr),
	m_fireSprite(nullptr),
	m_dockingText(nullptr),
	m_upgradedText(nullptr),
	m_font(nullptr),
	m_player(),
	m_dockLoad(0),
	m_music(nullptr)
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

		m_shipSprite = al_load_bitmap(player.getShip().getShipSprite().c_str()); //converting to c-style string because allegro is archaic or something...
		if(!m_shipSprite) { dockFail("Failed to load the shipSprite image"); }
		m_fireSprite = al_load_bitmap(player.getShip().getFireSprite().c_str());
		if(!m_fireSprite) { dockFail("Failed to load the fireball image"); }

		//Load audio
		m_music = al_load_sample("../../sounds/dock-music.wav");
		if (!m_music) { dockFail("Could not load 'dock-music.wav'.\n"); }
		al_play_sample(m_music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

		m_dockLoad = true;
	}
}

void Dock_display::unload()
{
	if(m_dockLoad)
	{
		if (m_dockingText) { al_destroy_bitmap(m_dockingText); m_dockingText=nullptr; }
		if (m_upgradedText) { al_destroy_bitmap(m_upgradedText); m_upgradedText=nullptr; }
		if (m_shipSprite) { al_destroy_bitmap(m_shipSprite); m_shipSprite=nullptr; }
		if (m_fireSprite) { al_destroy_bitmap(m_fireSprite); m_fireSprite=nullptr; }
		if(m_font) { al_destroy_font(m_font); m_font=nullptr; }
		if (m_music) { al_destroy_sample(m_music); m_music = nullptr; }
		m_failedUpgrade = false;
		m_dockLoad = false;
	}
}

void Dock_display::update()
{
	if(!m_dockLoad) { load(*m_player, *m_planet); }
//	al_clear_to_color(al_map_rgb(25,0,25)); //Draw purple background
//	al_draw_rotated_bitmap(m_shipSprite, al_get_bitmap_width(m_shipSprite)/2, al_get_bitmap_height(m_shipSprite)/2, m_windowWidth*0.48, m_windowHeight*0.4, 0, 0);
//	al_draw_rotated_bitmap(m_fireSprite, al_get_bitmap_width(m_fireSprite)/2, al_get_bitmap_height(m_fireSprite)/2, m_windowWidth*0.52, m_windowHeight*0.4, 0, 0);
//
////	al_draw_bitmap(m_dockingText, 0, 0, 0);
//	std::stringstream ss_planetText;
//	ss_planetText << "You have docked at " << m_planet->getPlanetName() << "."; //al_draw_text doesn't support newline (\n) so...
//	std::string planetText = ss_planetText.str();
//	al_draw_text(m_font, al_map_rgb(190,190,190), m_windowWidth*0.5, m_windowHeight*0.2, ALLEGRO_ALIGN_CENTRE, planetText.c_str());
//
//	std::string upgradeText;
//	if((*m_player).getShip().getUpgraded())
//	{
////		al_draw_bitmap(m_upgradedText, 0, m_windowHeight/2, 0);
//		upgradeText = "Your weapon has been upgraded.";
//	}
//	else if(!m_failedUpgrade) { upgradeText = "To upgrade your weapon, press W."; }
//	else { upgradeText = "Upgrading is impossible at this planet!"; }
//	al_draw_text(m_font, al_map_rgb(190,190,190), m_windowWidth*0.5, m_windowHeight*0.3, ALLEGRO_ALIGN_CENTRE, upgradeText.c_str());
	al_clear_to_color(al_map_rgb(25, 0, 25));
	al_draw_rotated_bitmap(al_load_bitmap(m_player->getShip().getShipSprite().c_str()), m_player->getShip().getWidth() / 2, m_player->getShip().getHeight() / 2, 250, 250, 0, 0);
	al_draw_rotated_bitmap(al_load_bitmap(m_player->getShip().getFireSprite().c_str()), 5, 5, 250, 250, 0, 0);
	al_draw_bitmap(al_load_bitmap("../../images/dockingText.png"), 0, 0, 0);
	std::stringstream s7;
	s7 << m_player->getShip().getCredits();
	std::string str7 = s7.str();
	/*al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.8, windowHeight*0.72, 0, str7.c_str());
	if (dispUpgradeText) al_draw_bitmap(upgradedText, 0, windowHeight / 2, 0);
	if (dispnotenoughcredits) {
		stringstream s6;
		s6 << "Not enough credits!";
		string str6 = s6.str();
		al_draw_text(font, al_map_rgb(255, 255, 255), windowWidth*0.05, windowHeight*0.9, 0, str6.c_str());
	}*/
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
	std::cout << failMessage << std::endl;
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
