#ifndef LOAD_GAME_DISPLAY_H
#define LOAD_GAME_DISPLAY_H

#include "Header.h"

class Load_game_display
{
private:
	bool m_loadGameLoad;
	int m_windowWidth,
		m_windowHeight,
		m_homeScreenOption;
	ALLEGRO_FONT *m_font;
	ALLEGRO_SAMPLE *m_music;
	std::vector<std::string> savedGameName;
	std::vector<int> savedGameSelected;

public:
	Load_game_display(int windowWidth, int windowHeight);
	void load();
	void unload();
	void update();
	void setHomeScreenOption(int homeScreenOption);
	int getNumberSaveGames();
	void loadGameFail(std::string failMessage);
	~Load_game_display(void);
};

#endif