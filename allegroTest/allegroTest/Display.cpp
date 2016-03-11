#include "Display.h"

Display::Display(void)
	: numberGrids(1000000), 
	shipSprite(al_load_bitmap("c:/dev/allegro/images/shipSprite.png")),	
	shipSprite1(al_load_bitmap("c:/dev/allegro/images/shipSprite1.png")),
	shipSprite2(al_load_bitmap("c:/dev/allegro/images/shipSprite2.png")), 
	fireSprite(al_load_bitmap("c:/dev/allegro/images/fireball.png")),
	backgroundSprite(al_load_bitmap("c:/dev/allegro/images/backgroundSprite3.png")), 
	planet1(al_load_bitmap("c:/dev/allegro/images/sun.png")),
	planet2(al_load_bitmap("c:/dev/allegro/images/mercury.png")), 
	planet3(al_load_bitmap("c:/dev/allegro/images/venus.png")),
	planet4(al_load_bitmap("c:/dev/allegro/images/earth.png")), 
	planet5(al_load_bitmap("c:/dev/allegro/images/mars.png")),
	planet6(al_load_bitmap("c:/dev/allegro/images/jupiter.png")), 
	planet7(al_load_bitmap("c:/dev/allegro/images/saturn.png")),
	planet8(al_load_bitmap("c:/dev/allegro/images/neptune.png")), 
	planet9(al_load_bitmap("c:/dev/allegro/images/uranus.png")),
	dockingText(al_load_bitmap("c:/dev/allegro/images/dockingText.png")),
	upgradedText(al_load_bitmap("c:/dev/allegro/images/upgradedText.png")),
	radarSprite(al_load_bitmap("c:/dev/allegro/images/radar1.png")),
	radarDotSprite(al_load_bitmap("c:/dev/allegro/images/radarDot.png")),
	radarScreenWidth(1920),
	radarScreenHeight(1080),
	radarScale(0.005)
{
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data); //Set resolution to max
	//windowWidth = backgroundWidth;
	//windowHeight = backgroundHeight;
	//windowWidth = 700;
	//windowHeight = 700;
	windowWidth = disp_data.width;
	windowHeight = disp_data.height;
	maxX = windowWidth*numberGrids;
	maxY = windowHeight*numberGrids;
    //al_set_new_display_flags(ALLEGRO_FULLSCREEN);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(windowWidth, windowHeight);

	//Radar: make sure this is created **after** window dimensions have been set as above
	radarBuffer = al_create_bitmap(windowWidth*3, windowHeight*3);

	if(!display) { displayFail("Failed to create display"); }

	//Check if bitmaps loaded properly
	if(!shipSprite) { displayFail("Failed to load the shipSprite image"); }
	if(!shipSprite1) { displayFail("Failed to load the shipSprite1 image"); }
	if(!shipSprite2) { displayFail("Failed to load the shipSprite2 image"); }
	if(!fireSprite) { displayFail("Failed to load the fireball image"); }
	if(!backgroundSprite) { displayFail("Failed to load the background image"); }
	if(!planet1) { displayFail("Failed to load the planet1 image"); }
	if(!planet2) { displayFail("Failed to load the planet2 image"); }
	if(!planet3) { displayFail("Failed to load the planet3 image"); }
	if(!planet4) { displayFail("Failed to load the planet4 image"); }
	if(!planet5) { displayFail("Failed to load the planet5 image"); }
	if(!planet6) { displayFail("Failed to load the planet6 image"); }
	if(!planet7) { displayFail("Failed to load the planet7 image"); }
	if(!planet8) { displayFail("Failed to load the planet8 image"); }
	if(!planet9) { displayFail("Failed to load the planet9 image"); }
	if(!dockingText) { displayFail("Failed to load the dockingText image"); }
	if(!upgradedText) { displayFail("Failed to load the upgradedText image"); }
	if(!radarSprite) { displayFail("Failed to load the radar image"); }
	if(!radarDotSprite) { displayFail("Failed to load the radar dot image"); }
}

void Display::update(Logic &logic, std::vector<Player> &players, Map &map)
	{
	switch (logic.getScreenMode()) 
	{
	//Home screen mode
	case 0:
		menu.update(logic.getMenu().getHomeScreenOption(), windowWidth, windowHeight);
		break;
	//Space mode
	case 1:
		space.update(logic, players, map);
		break;

	//Docking mode
	case 2:
		dock.update(logic.getDock().getDispUpgradeText());
		break;

	//Join server mode
//	case 3:
//		break;
	}
}

void Display::displayFail(std::string failMessage)
{
    printf("%s \n", failMessage);
	_getch();
    exit(1);
}

ALLEGRO_DISPLAY* Display::getDisplay() {return display; }
Menu_display& Display::getMenu() { return menu; }
Space_display& Display::getSpace() { return space; }
Dock_display& Display::getDock() { return dock; }

Display::~Display(void)
{
}
