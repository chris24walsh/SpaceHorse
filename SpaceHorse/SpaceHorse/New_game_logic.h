#ifndef NEW_GAME_LOGIC_H
#define NEW_GAME_LOGIC_H

#include "Header.h"

#include "New_game_display.h"
#include "Game_mode.h"

class New_game_logic
{
private:
	New_game_display *m_new_game;
	std::string m_textEntered;
	int m_newKey;
	char m_newChar;
	bool *m_gameStarted;

public:
	New_game_logic();
	void load(New_game_display &newGame, bool &gameStarted);
	void unload();
	void update();
	GameMode keyPress(ALLEGRO_EVENT &keyPressed);
	void enterText(char newKey);
	void deleteText();
	void fail(std::string message);
	~New_game_logic(void);
};

#endif