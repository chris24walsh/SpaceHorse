#ifndef SPACE_JUNK_H
#define SPACE_JUNK_H

#include "Header.h"

class SpaceJunk
{
private:
	int m_x;
	int m_y;
	ALLEGRO_BITMAP* m_sprite;

public:
	SpaceJunk();
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	void generateCoordinates();
	ALLEGRO_BITMAP *getSprite();
	~SpaceJunk();
};

#endif
