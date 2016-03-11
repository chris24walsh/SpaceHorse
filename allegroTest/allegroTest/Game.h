#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <conio.h>
#include <sstream>
#include <string>
#include <vector>

#include "Allegro_init.h"
#include "Enum.h"
#include "Player.h"
#include "Map.h"
#include "Display.h"
#include "Logic.h"

class Game {
private:
	std::vector<Player> players;
	Map map;
	Display display;
	Logic logic;
	ALLEGRO_EVENT_QUEUE *event_queue;
	ALLEGRO_TIMER *timer;
	const int FPS;

public:
	Game();
	void gameLoop();
	void abortGame(std::string message);
	~Game();
};

#endif