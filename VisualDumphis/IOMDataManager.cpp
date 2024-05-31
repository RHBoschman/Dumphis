#include "IOMDataManager.h"

IOMDataManager::IOMDataManager() : log("IOMDataManager") {}

void IOMDataManager::createData(const std::string& path) {
	log.logInfo("Start parsing IOM names...");
	tls::File file(path);
	std::string line;

	int index = 0;
	while (file.readLine2(line)) {
		if (containsStr(line, "IOM_") ||
			containsStr(line, "AI_") ||
			containsStr(line, "AIN_") ||
			containsStr(line, "THERMO_")
			) {
			index++;

			std::string str = line;
			DefineDataObject element;
			element.setValue(getNumber(str));
			element.setName(getName(str));

			data.push_back(element);
		}
	}

	log.logInfo(std::format("Found {} IOM elements", index));

#if PRINT_ALL
	for (DefineDataObject e : data) {
		e.printData();
	}
#endif
}

std::string IOMDataManager::getName(std::string& str) {
	std::string::iterator end_pos = std::remove(str.begin(), str.end(), ' ');
	str.erase(end_pos, str.end());
	std::string result = "";

	if (!str.empty()) {
		result = str;
	}
	else {
		log.logError("Could not find name");
		return result;
	}

	return result;
}

unsigned int IOMDataManager::getNumber(std::string& str) {
	unsigned int result = (~0);
	std::string resText = "";

	size_t pos_comma = str.find(',');
	if (pos_comma != std::string::npos) {
		resText = str.substr(0, pos_comma);
		std::string::iterator end_pos = std::remove(resText.begin(), resText.end(), ' ');
		resText.erase(end_pos, resText.end());
	}
	else {
		log.logError("No comma found");
		return result;
	}

	try {
		result = std::stoi(resText);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting define value {} to int went wrong: {}", str, e.what()));
		exit(0);
	}

	str = str.substr(pos_comma + 1);

	return result;
}