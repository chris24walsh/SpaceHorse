#ifndef SAVE_GAME_DISPLAY_H
#define SAVE_GAME_DISPLAY_H

#include "Header.h"

class Save_game_display
{
private:
	bool m_saveGameLoad;
	int m_windowWidth,
		m_windowHeight,
		m_homeScreenOption;
	ALLEGRO_FONT *m_font;
	ALLEGRO_SAMPLE *m_music;
	std::vector<std::string> savedGameName;
	std::vector<int> savedGameSelected;

public:
	Save_game_display(int windowWidth, int windowHeight);
	void load();
	void unload();
	void update();
	void setHomeScreenOption(int homeScreenOption);
	int getNumberSaveGames();
	std::vector<std::string> getSavedGames();
	void saveGameFail(std::string failMessage);
	~Save_game_display(void);
};

#endif