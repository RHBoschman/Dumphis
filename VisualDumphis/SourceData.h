#pragma once

#include <string>
#include <vector>
#include <iostream>

struct STEP {
	std::string name = "";
};

struct CMND {
	std::string name = "";
	std::vector<STEP> steps;
};

class SourceData {
public:
	void setFoundFunction(void);
	void setFuncIndex(const int index);
	void setFilePath(const std::string& path);
	void addCmnd(const CMND cmnd);
	void printData(void);
	int getFuncIndex(void) const;

private:
	bool foundFunction = false;
	int funcIndex = 0;
	std::vector<CMND> cmnds;
	std::string filePath = "";
};

