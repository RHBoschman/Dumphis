#include "DumpHisManager.h"

DumpHisManager::DumpHisManager() : log("DumpHisManager") {}

void DumpHisManager::createData(const std::string& path) {
	log.logInfo("Start creating dumphis data...");
	tls::File file(path);
	std::string line;

	int index = 0;
	while (file.readLine2(line)) {
		std::string str = line;
		DumpHis element;

		element.setTime(readTime(str));
		element.setUnitId(readUnit(str));
		element.setCmdId(readCmd(str));
		element.setState(readState(str));
		element.setStepId(readStep(str));
		element.setAlarmId(readAlarm(str));
		data.push_back(element);
		index++;
	}

	log.logInfo(std::format("Found {} dumphis data elements", index));

#if PRINT_ALL
	for (DumpHis e : data) {
		e.printData();
		log.blankLine();
	}
#endif
}

std::string DumpHisManager::readTime(std::string& str) {
	std::string result, trash;
	size_t pos = readUntilComma(str, result);
	str = str.substr(pos + 1);

	// Ignore next column (internal time)
	pos = readUntilComma(str, trash);
	str = str.substr(pos + 1);

	return result;
}

int DumpHisManager::readUnit(std::string& str) {
	std::string resText;
	size_t pos = readUntilComma(str, resText);
	str = str.substr(pos + 1);

	int result = 0;
	try {
		result = std::stoi(resText);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting unit {} to int went wrong: {}", resText, e.what()));
		exit(0);
	}

	return result;
}

int DumpHisManager::readCmd(std::string& str) {
	std::string resText;
	size_t pos = readUntilComma(str, resText);
	str = str.substr(pos + 1);

	int result = 0;
	try {
		result = std::stoi(resText);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting cmd {} to int went wrong: {}", resText, e.what()));
		exit(0);
	}

	return result;
}

int DumpHisManager::readState(std::string& str) {
	std::string resText, trash;
	size_t pos = readUntilComma(str, resText);
	str = str.substr(pos + 1);

	int result = 0;
	try {
		result = std::stoi(resText);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting state {} to int went wrong: {}", resText, e.what()));
		exit(0);
	}

	// Ignore next column (schedule)
	pos = readUntilComma(str, trash);
	str = str.substr(pos + 1);

	return result;
}

int DumpHisManager::readStep(std::string& str) {
	std::string resText;
	size_t pos = readUntilComma(str, resText);
	str = str.substr(pos + 1);

	int result = 0;
	try {
		result = std::stoi(resText);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting step {} to int went wrong: {}", resText, e.what()));
		exit(0);
	}

	return result;
}

int DumpHisManager::readAlarm(std::string& str) {
	int result = 0;
	try {
		result = std::stoi(str);
	}
	catch (std::exception& e) {
		log.logFatal(std::format("Converting alarm {} to int went wrong: {}", str, e.what()));
		exit(0);
	}

	return result;
}

size_t DumpHisManager::readUntilComma(const std::string& src, std::string& dst) {
	std::string result = "";
	size_t pos_comma = src.find(',');
	if (pos_comma != std::string::npos) {
		result = src.substr(0, pos_comma);
	}
	else {
		log.logError("No comma found");
	}

	dst = result;
	return pos_comma;
}