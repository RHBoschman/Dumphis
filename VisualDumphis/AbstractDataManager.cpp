#include "AbstractDataManager.h"


AbstractDataManager::AbstractDataManager() : log("AbstractDataManager") {}

std::vector<DefineDataObject>* AbstractDataManager::getData(void) {
	return &data;
}

bool AbstractDataManager::containsStr(const std::string& line, const std::string& str) {
	if (line.find(str) != std::string::npos)
		return true;
	return false;
}

std::string AbstractDataManager::getName(std::string& str) {
	std::string result = "";
	size_t pos_space = str.find(' ');
	if (pos_space != std::string::npos) {
		result = str.substr(0, pos_space);
		str = str.substr(pos_space);
	}
	else {
		log.logError("Could not find name");
		return result;
	}

	return result;
}

int AbstractDataManager::getNumber(std::string& str) {
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());

	int result = -1;
	try {
		result = std::stoi(str);
	}
	catch (std::exception& e) {
		log.logError(std::format("Converting define value {} to int went wrong: {}", str, e.what()));
		//log.logFatal(std::format("Converting define value {} to int went wrong: {}", str, e.what()));
		//exit(0);
	}

	return result;
}

void AbstractDataManager::removeTabs(std::string& str) {
	size_t pos = str.find('\t');
	while (pos != std::string::npos) {
		str.erase(pos, 1);
		pos = str.find('\t', pos);
	}
}