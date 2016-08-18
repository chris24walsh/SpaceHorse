#ifndef NEW_GAME_DISPLAY_H
#define NEW_GAME_DISPLAY_H

#include "Header.h"

class New_game_display
{
private:
	bool m_newGameLoad; //Whether or not the new_game_display object has been loaded
	int m_windowWidth,
		m_windowHeight;
	std::string	m_textEntered;
	ALLEGRO_FONT *m_font;
	ALLEGRO_SAMPLE *m_music;

public:
	New_game_display(int windowWidth, int windowHeight);
	void load();
	void unload();
	void update(); //Redraw the display for the 'NewGame' state
	void setTextEntered(std::string textEntered);
	void menuFail(std::string failMessage);
	~New_game_display(void);
};

#endif