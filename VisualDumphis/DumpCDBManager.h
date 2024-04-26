#pragma once

#include <vector>
#include "tools/tls.h";
#include "DumpCDB.h"

class DumpCDBManager
{
public:
	void createData(const std::string& path);
	std::vector<DumpCDB> getData(void);
private:
	std::vector<DumpCDB> data;

	int getE_CDB(const std::string& line);
	std::string& getE_FUNCTION(const std::string& line);
	std::vector<int> getE_DATA(const std::string& line);
	bool containsStr(const std::string& line, const std::string& str);
	std::string& getAfterComma(const std::string& line);
};

