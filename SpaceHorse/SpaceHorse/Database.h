#pragma once

#include "Header.h"
#include "Player.h"

class Database
{
private:
	int rc;
	char *error;
	sqlite3 *db;
public:
	Database(std::string fileName); //Open a database connection - takes filename
	void save(Player &player); //Set some data - takes key value pair
	std::string load(Player &player); //Get some data - takes key, returns value
	~Database(); //Closes database connection
};