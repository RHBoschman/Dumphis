#pragma once

#include "AbstractDataManager.h"

#define PRINT_ALL 0

class CDBDataManager : public AbstractDataManager {
public:
	CDBDataManager();
	void createData(const std::string& path) override;

private:
	tls::LogManager log;
};

