#pragma once

#include "Header.h"

class Database
{
private:
	int rc;
	char *error;
	sqlite3 *db;
public:
	Database(std::string fileName); //Open a database connection - takes filename
	void set(std::string key, std::string value); //Set some data - takes key value pair
	std::string get(std::string key); //Get some data - takes key, returns value
	~Database(); //Closes database connection
};