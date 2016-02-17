#include "Game.h"
#include "Ship.h"
#include "Planet.h"

using namespace std;

int main(int argc, char* argv[])
{
	//Create a game object
	Game newGame;

	//Create player 1, and add to game
	Ship player1;
	newGame.ships.push_back(player1);

	//Create solar system
	srand(6); //Set a known random nonce before seeding planets, so I know how the planets will be dispersed "randomly" :)
	Planet planet1("c:/dev/allegro/images/sun.png", 0);
	Planet planet2("c:/dev/allegro/images/mercury.png", 1);
	Planet planet3("c:/dev/allegro/images/venus.png", 2);
	Planet planet4("c:/dev/allegro/images/earth.png", 3);
	Planet planet5("c:/dev/allegro/images/mars.png", 4);
	Planet planet6("c:/dev/allegro/images/jupiter.png", 13);
	Planet planet7("c:/dev/allegro/images/saturn.png", 24);
	Planet planet8("c:/dev/allegro/images/neptune.png", 49);
	Planet planet9("c:/dev/allegro/images/uranus.png", 76);
	newGame.planets.push_back(planet1);
	newGame.planets.push_back(planet2);
	newGame.planets.push_back(planet3);
	newGame.planets.push_back(planet4);
	newGame.planets.push_back(planet5);
	newGame.planets.push_back(planet6);
	newGame.planets.push_back(planet7);
	newGame.planets.push_back(planet8);
	newGame.planets.push_back(planet9);

	//Start game
	newGame.game_loop();
}