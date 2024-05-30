#pragma once

#include <vector>
#include <format>
#include "tools\tls.h"
#include "DefineDataObject.h"

#define PRINT_ALL 0

class CDBDataManager
{
public:
	CDBDataManager();
	void createData(const std::string& path);
	std::vector<DefineDataObject> getData(void);

private:
	tls::LogManager log;
};

