#include "Map.h"


Map::Map(void)
	:planets(0)
{
}

void Map::makeSolarSystem()
{
	Planet sun("c:/dev/allegro/images/sun.png", 0);
	Planet mercury("c:/dev/allegro/images/mercury.png", 1);
	Planet venus("c:/dev/allegro/images/venus.png", 2);
	Planet earth("c:/dev/allegro/images/earth.png", 3);
	Planet mars("c:/dev/allegro/images/mars.png", 4);
	Planet jupiter("c:/dev/allegro/images/jupiter.png", 13);
	Planet saturn("c:/dev/allegro/images/saturn.png", 24);
	Planet neptune("c:/dev/allegro/images/neptune.png", 49);
	Planet uranus("c:/dev/allegro/images/uranus.png", 76);
	planets.reserve(9); //this is the number of celestial bodies, but if it's wrong, push_back will still resize if necessary
	planets.push_back(sun);
	planets.push_back(mercury);
	planets.push_back(venus);
	planets.push_back(earth);
	planets.push_back(mars);
	planets.push_back(jupiter);
	planets.push_back(saturn);
	planets.push_back(neptune);
	planets.push_back(uranus);
}

std::vector<Planet>& Map::getPlanets() { return planets; }

Map::~Map(void)
{
}
