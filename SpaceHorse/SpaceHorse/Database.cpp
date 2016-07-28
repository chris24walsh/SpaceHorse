#include "Database.h"

Database::Database(std::string fileName)
{
	// Open Database
	std::cout << "Opening MyDb.db ..." << std::endl;
	std::string saveName = "../../Saves/" + fileName + ".db";
	rc = sqlite3_open(saveName.c_str(), &db);
	if (rc)
	{
		std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_close(db);
	}
	else
	{
		std::cout << "Opened MyDb.db." << std::endl << std::endl;
	}

	//Create table
	std::cout << "Creating MyTable ..." << std::endl;
	const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Created MyTable." << std::endl << std::endl;
	}
}

void Database::set(std::string key, std::string value)
{
	//Insert data
	std::cout << "Inserting a value into MyTable ..." << std::endl;
	const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
	rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	if (rc)
	{
		std::cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << std::endl << std::endl;
		sqlite3_free(error);
	}
	else
	{
		std::cout << "Inserted a value into MyTable." << std::endl << std::endl;
	}
}

std::string Database::get(std::string key)
{
	//Get data
	std::cout << "Retrieving values in MyTable ..." << std::endl;
	const char *sqlSelect = "SELECT * FROM MyTable;";
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
		// Display Table
		for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
		{
			for (int colCtr = 0; colCtr < columns; ++colCtr)
			{
				// Determine Cell Position
				int cellPosition = (rowCtr * columns) + colCtr;

				// Display Cell Value
				std::cout.width(12);
				std::cout.setf(std::ios::left);
				std::cout << results[cellPosition] << " ";
			}

			// End Line
			std::cout << std::endl;

			// Display Separator For Header
			if (0 == rowCtr)
			{
				for (int colCtr = 0; colCtr < columns; ++colCtr)
				{
					std::cout.width(12);
					std::cout.setf(std::ios::left);
					std::cout << "~~~~~~~~~~~~ ";
				}
				std::cout << std::endl;
			}
		}
	}
	sqlite3_free_table(results);

	return "Test";
}

Database::~Database()
{
	// Close Database
	std::cout << "Closing MyDb.db ..." << std::endl;
	sqlite3_close(db);
	std::cout << "Closed MyDb.db" << std::endl << std::endl;
}
