#pragma once

#include "AbstractDataManager.h"

#define PRINT_ALL 0

class IOMDataManager : public AbstractDataManager {
public:
	IOMDataManager();
	void createData(const std::string& path) override;

private:
	tls::LogManager log;

	std::string getName(std::string& str) override;
	int getNumber(std::string& str) override;
};

