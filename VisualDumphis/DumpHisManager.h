#pragma once

#include <vector>
#include <format>
#include "tools/tls.h";
#include "DumpHis.h"

#define PRINT_ALL 0

class DumpHisManager
{
public:
	DumpHisManager();
	void createData(const std::string& path);
	//std::vector<DumpCDB> getData(void);

private:
	tls::LogManager log;
	std::vector<DumpHis> data;

	std::string readTime(std::string& str);
	int readUnit(std::string& str);
	int readCmd(std::string& str);
	int readState(std::string& str);
	int readStep(std::string& str);
	int readAlarm(std::string& str);
	size_t readUntilComma(const std::string& src, std::string& dst);
};

