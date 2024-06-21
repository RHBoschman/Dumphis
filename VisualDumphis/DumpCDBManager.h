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
	std::vector<DumpCDB>* getData(void);
	std::vector<std::string> getFuncNames(void);

private:
	tls::LogManager log;
	std::vector<DumpCDB> data;
	std::vector<std::string> funcIndexes;

	int getE_CDB(const std::string& line);
	std::string getE_FUNCTION(const std::string& line);
	std::vector<int> getE_DATA(const std::string& line);
	bool containsStr(const std::string& line, const std::string& str);
	std::string getAfterComma(const std::string& line);
	int getDataValue(std::string& str);
	void createFuncIndexes(void);
};

