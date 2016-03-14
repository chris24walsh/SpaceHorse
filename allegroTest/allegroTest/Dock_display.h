#ifndef DOCK_DISPLAY_H
#define DOCK_DISPLAY_H

#include "Header.h"

#include "Player.h"

class Dock_display
{
private:
	bool m_dispUpgradeText,
		m_dockLoad;
	int m_windowWidth,
		m_windowHeight;
	ALLEGRO_BITMAP *m_shipSprite,
		*m_fireSprite,
		*m_dockingText,
		*m_upgradedText;
	Player *m_player;

public:
	Dock_display(int windowWidth, int windowHeight);
	void load(Player &player);
	void unload();
	void update();
	void setDispUpgradeText();
	int getFireSpriteWidth();
	int getFireSpriteHeight();
	void dockFail(std::string failMessage);
	~Dock_display(void);
};

#endif