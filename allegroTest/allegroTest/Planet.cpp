#include "Planet.h"

Planet::Planet()
	:m_x(0),
	m_y(0),
	m_height(0),
	m_width(0),
	m_color1(0),
	m_color2(0),
	m_color3(0),
	m_planetScale(0),
	m_distance(0),
	m_canCollide(0),
	m_canDock(0),
	m_planetSprite("")
{
}

Planet::Planet(std::string planetSprite)
	:m_x(rand()%1000),
	m_y(rand()%1000),
	m_height(0),
	m_width(0),
	m_color1(0),
	m_color2(0),
	m_color3(0),
	m_distance(0),
	m_canCollide(0),
	m_canDock(0),
	m_planetSprite(planetSprite),
	m_planetScale(4)
{
	//logic will set height and width when loaded by taking the info from display
}

Planet::Planet(std::string planetSprite, int distance)
	:m_x(0),
	m_y(0),
	m_height(0),
	m_width(0),
	m_canCollide(0),
	m_canDock(true),
	m_planetScale(4),
	m_distance(distance),
	m_planetSprite(planetSprite),
	m_color1(255),
	m_color2(255),
	m_color3(255)
{
	double angle = (6.25)*(rand()%100)/100;
	m_x = distance * 1000 * m_planetScale * cos(angle) + 500000;
	m_y = distance * 1000 * m_planetScale * sin(angle) + 500000;
	//logic will set height and width when loaded by taking the info from display
}

int Planet::getX() { return m_x; }
int Planet::getY() { return m_y; }
int Planet::getPlanetScale() { return m_planetScale; }
int Planet::getColor1() { return m_color1; }
int Planet::getColor2() { return m_color2; }
int Planet::getColor3() { return m_color3; }
int Planet::getWidth() { return m_width; }
int Planet::getHeight() { return m_height; }
bool Planet::getCanDock() { return m_canDock; }
std::string Planet::getPlanetSprite() { return m_planetSprite; }
void Planet::setPlanetSize(int planetWidth, int planetHeight) { m_width = planetWidth; m_height = planetHeight; }

Planet::~Planet()
{
}