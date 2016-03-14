#ifndef SPACE_DISPLAY_H
#define SPACE_DISPLAY_H

#include "Header.h"

#include "Player.h"
#include "Map.h"

class Space_display
{
private:
	bool m_gameOver,
		m_hyperDrive,
		m_spaceLoad;
	int m_test,
		m_numberGrids,
		m_gridX,
		m_gridY,
		m_bgX,
		m_bgY,
		m_screenWidth,
		m_screenHeight,
		m_radarScreenWidth,
		m_radarScreenHeight,
		m_windowWidth,
		m_windowHeight;
	std::vector<int> m_shipSpriteCurrents; //values 0, 1, or 2 for each player to keep track of current frame of ship animation
	double m_radarScale;
	std::string m_editText;
	ALLEGRO_BITMAP *m_backgroundSprite,
		*m_radarSprite,
		*m_radarDotSprite,
		*m_radarBuffer;
	std::vector<ALLEGRO_BITMAP*> m_planetSprites,
		m_shipSprites,
		m_shipSprite1s,
		m_shipSprite2s,
		m_fireSprites;
	std::vector<Player> *m_players;
	Map *m_map;
	ALLEGRO_FONT *font;

public:
	Space_display(int windowWidth, int windowHeight);
	void load(std::vector<Player> &players, Map &map);
	void unload();
	void update();
	int getPlanetSpriteWidth(int index);
	int getPlanetSpriteHeight(int index);
	int getShipSpriteWidth(int index);
	int getShipSpriteHeight(int index);
	int getFireSpriteWidth(int index);
	int getFireSpriteHeight(int index);
	int getNumberGrids();
	void setGridX();
	void setGridY();
	void setBgX();
	void setBgY();
	void setEditText(std::string editText);
	void setGameOver();
	void toggleHyperDrive();
	void setShipSpriteCurrents(int index, int shipSpriteCurrent);
	void spaceFail(std::string failMessage);
	~Space_display(void);
};

#endif