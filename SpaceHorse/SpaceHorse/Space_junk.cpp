#include "Space_junk.h"

SpaceJunk::SpaceJunk(void):
	m_x((rand() % 20000) + 490000),
	m_y((rand() % 20000) + 490000),
	m_sprite(al_load_bitmap("../../images/Space_junk.png"))
{
	printf("Space Junk Generated at %d, %d\n", m_x, m_y);
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
	return al_get_bitmap_width(m_sprite);
}

int SpaceJunk::getHeight(void)
{
	return al_get_bitmap_height(m_sprite);
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
