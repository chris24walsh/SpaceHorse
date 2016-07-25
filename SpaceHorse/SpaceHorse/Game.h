#ifndef GAME_H
#define GAME_H

#include "Header.h"

#include "Display.h"
#include "Logic.h"

class Game {
private:
	Display display;
	Logic logic;
	void gameLoop(ALLEGRO_TIMER &timer, ALLEGRO_EVENT_QUEUE &event_queue);

public:
	Game(int width, int height, ALLEGRO_TIMER &timer, ALLEGRO_EVENT_QUEUE &event_queue);
	~Game();
};

#endif