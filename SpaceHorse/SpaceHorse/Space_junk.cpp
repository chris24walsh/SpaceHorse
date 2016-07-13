#include "Space_junk.h"

SpaceJunk::SpaceJunk(void)
{
	m_x = (rand() % 40000) + 480000;
	m_y = (rand() % 40000) + 480000;
	m_sprite = al_load_bitmap("../../images/Space_junk.png");
	int width = al_get_bitmap_width(m_sprite);
	if (!m_sprite) 
	{ 
		printf("Could not load space junk sprite.\n");
		_getch();
		exit(1);
	}
}

int SpaceJunk::getX(void)
{
	return m_x;
}

int SpaceJunk::getY(void)
{
	return m_y;
}

int SpaceJunk::getWidth(void)
{
	return 120;
}

int SpaceJunk::getHeight(void)
{
	return 120;
}

ALLEGRO_BITMAP * SpaceJunk::getSprite()
{
	return m_sprite;
}

SpaceJunk::~SpaceJunk(void) {
	if (m_sprite)
	{
		al_destroy_bitmap(m_sprite);
	}
}
