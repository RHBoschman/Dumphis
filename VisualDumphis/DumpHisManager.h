#pragma once

#include <vector>
#include <format>
#include <sstream>
#include "tools/tls.h";
#include "DumpHis.h"

#define PRINT_ALL 0

class DumpHisManager
{
public:
	DumpHisManager();
	void createData(const std::string& path);
	std::vector<DumpHis>* getData(void);

private:
	tls::LogManager log;
	std::vector<DumpHis> data;
	long long int uniqueId = 0;

	std::string readTime(std::string& str);
	int readUnit(std::string& str);
	int readCmd(std::string& str);
	int readState(std::string& str);
	int readStep(std::string& str);
	int readAlarm(std::string& str);
	size_t readUntilComma(const std::string& src, std::string& dst);
	std::string getUniqueId(void);
};

