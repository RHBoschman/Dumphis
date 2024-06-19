#pragma once

#include <string>
#include <vector>
#include <iostream>

struct STEP {
	std::string name = "";
};

struct FALL_CMND {
	std::string name = "";
	int CmndIndex = -1;
};

struct CMND {
	std::string name = "";
	std::vector<STEP> steps;
	std::vector<FALL_CMND> fallCmnds;
	int CmndIndex = -1;
};

class SourceData {
public:
	void setFoundFunction(void);
	void setFuncIndex(const int index);
	void setFilePath(const std::string& path);
	void setCmndIndex(const int i, const int index);
	void setFallCmndIndex(const int i, const int j,const int index);
	void addCmnd(const CMND cmnd);
	void printData(void);
	int getFuncIndex(void) const;
	std::vector<CMND> getCMNDs(void);

private:
	bool foundFunction = false;
	int funcIndex = 0;
	std::vector<CMND> cmnds;
	std::string filePath = "";
};

