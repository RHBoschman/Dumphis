#pragma once

#include <vector>
#include <format>
#include "tools\tls.h"
#include "DefineDataObject.h"

class AbstractDataManager
{
public:
	virtual void createData(const std::string& path) = 0;
	virtual std::vector<DefineDataObject> getData(void);

private:
	tls::LogManager log;
	std::vector<DefineDataObject> data;

	virtual bool containsStr(const std::string& line, const std::string& str);
	std::string getName(std::string& str);
	unsigned int getNumber(std::string& str);
};

