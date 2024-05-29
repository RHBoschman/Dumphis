#pragma once

#include <vector>
#include <format>
#include "tools/tls.h";
#include "DumpCDB.h"

#define MAX_DATA_ELEMENTS 8
#define PRINT_ALL 0

class DumpCDBManager {
public:
	DumpCDBManager();
	void createData(const std::string& path);
	std::vector<DumpCDB> getData(void);

private:
	tls::LogManager log;
	std::vector<DumpCDB> data;

	int getE_CDB(const std::string& line);
	std::string getE_FUNCTION(const std::string& line);
	std::vector<int> getE_DATA(const std::string& line);
	bool containsStr(const std::string& line, const std::string& str);
	std::string getAfterComma(const std::string& line);
	int getDataValue(std::string& str);
};

