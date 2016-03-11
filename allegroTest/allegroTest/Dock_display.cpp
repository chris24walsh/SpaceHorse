#include "Dock_display.h"


Dock_display::Dock_display(void)
{
}

void Dock_display::load()
{
	if(!dockingText) { displayFail("Failed to load the dockingText image"); }
	if(!upgradedText) { displayFail("Failed to load the upgradedText image"); }
}

void Dock_display::update()
{
	al_clear_to_color(al_map_rgb(25,0,25));
	al_draw_rotated_bitmap(shipSprite, (al_get_bitmap_width(shipSprite))/2, al_get_bitmap_height(shipSprite)/2, 250, 250, 0, 0);
	al_draw_rotated_bitmap(fireSprite, (al_get_bitmap_width(fireSprite))/2, (al_get_bitmap_height(fireSprite))/2, 250, 250, 0, 0);
	al_draw_bitmap(dockingText, 0, 0, 0);
	if (dispUpgradeText) al_draw_bitmap(upgradedText, 0, windowHeight/2, 0);
}

Dock_display::~Dock_display(void)
{
}
