#include "Game.h"

int main(int argc, char* argv[])
{
	srand(static_cast<unsigned int>(time(0))); //this is in effect globally, so start it here
	rand(); //junk first result because it is always about the same in Visual Studio
	//Create a game object which will itself start a game loop and later shutdown
	Game game;
}