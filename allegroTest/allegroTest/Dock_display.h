#ifndef DOCK_DISPLAY_H
#define DOCK_DISPLAY_H

#include "Header.h"

#include "Player.h"
#include "Planet.h"

class Dock_display
{
private:
	bool m_failedUpgrade,
		m_dockLoad;
	int m_windowWidth,
		m_windowHeight;
	ALLEGRO_BITMAP *m_fireSprite,
		*m_dockingText,
		*m_upgradedText;
	Player *m_player;
	Planet *m_planet;
	ALLEGRO_FONT *m_font;

public:
	Dock_display(int windowWidth, int windowHeight);
	void load(Player &player, Planet &planet);
	void unload();
	void update();
	void setDispUpgradeText();
	void setFailedUpgrade();
	int getFireSpriteWidth();
	int getFireSpriteHeight();
	void dockFail(std::string failMessage);
	~Dock_display(void);
};

#endif