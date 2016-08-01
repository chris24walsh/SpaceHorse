#include "Database.h"

Database::Database(std::string fileName)
{
	// Open Database
	//std::cout << "Opening MyDb.db ..." << std::endl;
	std::string saveName = "../../Saves/" + fileName;
	rc = sqlite3_open(saveName.c_str(), &db);
	if (rc)
	{
		std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_close(db);
	}
	else
	{
		//std::cout << "Opened MyDb.db." << std::endl << std::endl;
	}

	//Initialise database
	if (1) { //Put some condition here, otherwise the database grows every time its accessed
		//Create table
		//std::cout << "Creating MyTable ..." << std::endl;
		const char *sql1 = "CREATE TABLE Players (player INTEGER PRIMARY KEY, health INTEGER, x INTEGER, y INTEGER, angle FLOAT);";
		rc = sqlite3_exec(db, sql1, NULL, NULL, &error);
		if (rc)
		{
			std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
			sqlite3_free(error);
		}
		else
		{
			//std::cout << "Created MyTable." << std::endl << std::endl;
		}
		const char *sql2 = "INSERT INTO Players VALUES(NULL, 6, 490000, 500000, 0);";
		rc = sqlite3_exec(db, sql2, NULL, NULL, &error);
		if (rc)
		{
			std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
			sqlite3_free(error);
		}
		else
		{
			//std::cout << "Created MyTable." << std::endl << std::endl;
		}
	}
}

void Database::set(Player &player)
{
	//Insert data
	//std::cout << "Inserting a value into MyTable ..." << std::endl;
	std::string query = "UPDATE Players SET health=" + std::to_string(player.getShip().getHealth()) + ",x=" + std::to_string(player.getShip().getX()) + ",y=" + std::to_string(player.getShip().getY()) + ",angle=" + std::to_string(player.getShip().getAngle()) + " WHERE player=1;";
	rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		//std::cout << "Inserted a value into MyTable." << std::endl << std::endl;
	}
}

std::string Database::get(Player &player)
{
	//Get data
	std::cout << "Retrieving values in Players ..." << std::endl;
	const char *sqlSelect = "SELECT * FROM Players;";
	char **results = NULL;
	int rows, columns;
	sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		//Loading data into game
		player.getShip().setHealth(atoi(results[6]));
		player.getShip().setX(atof(results[7]));
		player.getShip().setY(atof(results[8]));
		player.getShip().setAngle(atof(results[9]));
		player.getShip().setSpeed(0);
	}

	sqlite3_free_table(results);

	return "test";
}

Database::~Database()
{
	// Close Database
	//std::cout << "Closing Database ..." << std::endl;
	sqlite3_close(db);
	//std::cout << "Closed Database" << std::endl << std::endl;
}
