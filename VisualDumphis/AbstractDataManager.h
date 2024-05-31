#pragma once

#include <vector>
#include <format>
#include "tools\tls.h"
#include "DefineDataObject.h"

class AbstractDataManager
{
public:
	AbstractDataManager();
	virtual void createData(const std::string& path) = 0;
	std::vector<DefineDataObject> getData(void);

protected:
	std::vector<DefineDataObject> data;

	bool containsStr(const std::string& line, const std::string& str);
	virtual std::string getName(std::string& str);
	virtual unsigned int getNumber(std::string& str);

private:
	tls::LogManager log;
};

