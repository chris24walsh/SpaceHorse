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
	int m_numberGrids,
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
	double m_radarScale;
	std::string m_editText;
	ALLEGRO_BITMAP *m_backgroundSprite,
		*m_radarSprite,
		*m_radarDotSprite,
		*m_radarBuffer;
	std::vector<ALLEGRO_BITMAP*> m_planetSprites,
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
	int getFireSpriteWidth(int index);
	int getFireSpriteHeight(int index);
	int getNumberGrids();
	void setGrid();
	void setEditText(std::string editText);
	void setGameOver();
	void setHyperDrive(bool hyperDrive);
	void spaceFail(std::string failMessage);
	~Space_display(void);
};

#endif