#include "DumpCDBManager.h"

DumpCDBManager::DumpCDBManager() : log("DumpCDBManager") {}

void DumpCDBManager::createData(const std::string& path) {
	log.logInfo("Start creating CDB data...");
	tls::File file(path);
	std::string line;
	bool inElement = false;

	unsigned int id;
	std::string function;
	std::vector<int> dataCons;
	int n_cmds = 0;
	int index = 0;
	while (file.readLine2(line)) {
		if (containsStr(line, "CDB")) {
			inElement = true;
			id = getE_CDB(line);
		}

		if (inElement) {
			if (containsStr(line, "FUNCTION"))
				function = getE_FUNCTION(line);
			else if (containsStr(line, "DATA"))
				dataCons = getE_DATA(line);
			else if (containsStr(line, "COMMAND"))
				n_cmds++;
		}

		if (line.empty()) {
			inElement = false;
			DumpCDB element;
			element.setId(id);
			element.setFunction(function);
			element.setConnections(dataCons);
			element.setCmds(n_cmds);
			data.push_back(element);
			n_cmds = 0;
#if PRINT_ALL
			log.logInfo(std::format("Processed CDB data with id {}", id));
#endif
			index++;
		}
	}

	log.logInfo(std::format("Found {} CDB data elements", index));

#if PRINT_ALL
	for (DumpCDB e : data) {
		e.printData();
		log.blankLine();
	}
#endif

	createFuncIndexes();
}

std::vector<DumpCDB>* DumpCDBManager::getData(void) {
	return &data;
}

std::vector<std::string> DumpCDBManager::getFuncNames(void) {
	return funcIndexes;
}

int DumpCDBManager::getE_CDB(const std::string& line) {
	unsigned int result = 0;
	try {
		result = std::stoi(getAfterComma(line));
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting CDB id to int went wrong: {}", e.what()));
		exit(0);
	}

	return result;
}

std::string DumpCDBManager::getE_FUNCTION(const std::string& line) {
	return getAfterComma(line);
}

std::vector<int> DumpCDBManager::getE_DATA(const std::string& line) {
	std::vector<int> result;
	int tmp = 0;
	std::string str = line;

	int index = 0;
	do {
		tmp = getDataValue(str);
		if (tmp < 0)
			break;

		result.push_back(tmp);
		index++;
	} while (index < MAX_DATA_ELEMENTS);

	return result;
}

bool DumpCDBManager::containsStr(const std::string& line, const std::string& str) {
	if (line.find(str) != std::string::npos)
		return true;
	return false;
}

std::string DumpCDBManager::getAfterComma(const std::string& line) {
	std::string text = "";

	size_t pos_comma = line.find(',');
	if (pos_comma != std::string::npos) {

		size_t pos_semicolon = line.find(';');
		if (pos_semicolon != std::string::npos) {
			text = line.substr(pos_comma + 1, (pos_semicolon - pos_comma - 1));
		}
		else {
			log.logError("No semicolon found");
		}
	}
	else {
		log.logError("No comma found");
	}

	return text;
}

int DumpCDBManager::getDataValue(std::string& str) {
	std::string val = "";
	int result = 0;

	size_t pos_comma = str.find(',');
	if (pos_comma != std::string::npos) {

		size_t pos_end = str.find(',', pos_comma + 1);
		if (pos_end != std::string::npos) {
			
			val = str.substr(pos_comma + 1, (pos_end - pos_comma - 1));
			str = str.substr(pos_end);
		}
		else {
			size_t pos_semicolon = str.find(';');
			if (pos_semicolon != std::string::npos) {
				val = str.substr(pos_comma + 1, (pos_semicolon - pos_comma - 1));
				str = str.substr(pos_semicolon);
			}
			else {
				log.logError("No semicolon found");
			}
		}
	}
	else {
		return -1; // End
	}

	try {
		result = std::stoi(val);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting data value to int went wrong: {}", e.what()));
		exit(0);
	}

	return result;
}

void DumpCDBManager::createFuncIndexes(void) {
	bool isPresent = false;
	for (DumpCDB e : data) {
		isPresent = false;
		std::string name = e.getFuncName();
		for (std::string f : funcIndexes) {
			if (name == f) {
				isPresent = true;
				break;
			}
		}

		if (!isPresent)
			funcIndexes.push_back(name);
	}

	log.logInfo("Created list of functions");
}