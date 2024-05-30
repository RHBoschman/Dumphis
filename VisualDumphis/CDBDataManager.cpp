#include "CDBDataManager.h"

CDBDataManager::CDBDataManager() : log("CDBDataManager") {}

void CDBDataManager::createData(const std::string& path) {
	log.logInfo("Start parsing Units/Entities names...");
	tls::File file(path);
	std::string line;

	int index = 0;
	while (file.readLine2(line)) {
		if (containsStr(line, "UNIT_") ||
			containsStr(line, "U_")	||
			containsStr(line, "E_")
			) {
			index++;
			std::string define = "#define";
			size_t pos_define = line.find(define);
			if (pos_define != std::string::npos) {
				std::string substr = line.substr((pos_define + define.length() + 1));

				DefineDataObject element;
				element.setName(getName(substr));
				element.setValue(getNumber(substr));
				
				data.push_back(element);
			}
			else {
				log.logError("'#define' not found");
			}
		}
	}

	log.logInfo(std::format("Found {} units/entities", index));

#if PRINT_ALL
	for (CDBData e : data) {
		e.printData();
	}
#endif
}

bool CDBDataManager::containsStr(const std::string& line, const std::string& str) {
	if (line.find(str) != std::string::npos)
		return true;
	return false;
}

std::string CDBDataManager::getName(std::string& str) {
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

unsigned int CDBDataManager::getNumber(std::string& str) {
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());

	unsigned int result;
	try {
		result = std::stoi(str);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting U/E value {} to int went wrong: {}", str, e.what()));
		exit(0);
	}

	return result;
}
