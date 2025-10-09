#pragma once
#include <list>
#include <string>
#include <vector>

class Utility {
public:
	static int LoadCSV(const char* file_name, std::vector<std::vector<std::string>>& array);
	static int LoadCSV(const char* file_name, std::list<std::list<std::string>>& array);

	static int SaveCSV(const char* file_name, std::vector<std::vector<std::string>>& array);
	static int SaveCSV(const char* file_name, std::list<std::list<std::string>>& array);

};

