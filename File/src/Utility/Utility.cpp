#include <fstream>
#include <sstream>
#include "Utility.h"

bool Utility::CheckFileExists(const char* file_name) {
	std::ifstream ifs = std::ifstream(file_name);
	if (ifs) return true;
	return false;
}

int Utility::LoadCSV(const char* file_name, std::vector<std::vector<std::string>>& array) {
	std::ifstream ifs = std::ifstream(file_name);

	if (!ifs) { return -1; }

	std::string line;

	while (getline(ifs, line)) {
		std::istringstream stream(line);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, ',')) {
			result.emplace_back(field);
		}
		array.emplace_back(result);
	}
	return array.empty() ? -2 : 0;
}

int Utility::LoadCSV(const char* file_name, std::list<std::list<std::string>>& array) {
	std::ifstream ifs = std::ifstream(file_name);

	if (!ifs) { return -1; }

	std::string line;

	while (getline(ifs, line)) {
		std::istringstream stream(line);
		std::string field;
		std::list<std::string> result;
		while (getline(stream, field, ',')) {
			result.emplace_back(field);
		}
		array.emplace_back(result);
	}
	return array.empty() ? -2 : 0;
}

int Utility::SaveCSV(const char* file_name, std::vector<std::vector<std::string>>& array) {
	std::ofstream ofs = std::ofstream(file_name);

	if (!ofs) { return -1; }

	for (int idx1 = 0; idx1 < array.size(); idx1++) {
		for (int idx2 = 0; idx2 < array[idx1].size(); idx2++) {
			ofs << array[idx1][idx2];
			if (idx2 != array[idx1].size() - 1) {
				ofs << ",";
			}
		}
		ofs << std::endl;
	}
	return 0;
}

int Utility::SaveCSV(const char* file_name, std::list<std::list<std::string>>& array) {
	std::ofstream ofs = std::ofstream(file_name);

	if (!ofs) { return -1; }

	for (auto idx1 = array.begin(); idx1 != array.end(); idx1++) {
		for (auto idx2 = (*idx1).begin(); idx2 != (*idx1).end(); idx2++) {
			ofs << *idx2;
			if (idx2 != --(*idx1).end()) {
				ofs << ",";
			}
		}
		ofs << std::endl;
	}
	return 0;
}
