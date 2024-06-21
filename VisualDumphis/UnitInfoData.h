#pragma once

#include <vector>
#include <string>

enum UnitTypes {
	UNIT,
	ENTITY,
	UNKNOWN
};

struct Step {
	std::string name = "";
};

struct Cmnd {
	int id = -1;
	std::string name = "";
	std::vector<Step> steps;
};

struct FallCmnd {
	int id = -1;
	std::string name = "";
	int refCmndId = -1;
};

struct UnitInfoDataObject {
	int id = -1;
	UnitTypes type = UNKNOWN;
	std::string name = "";
	std::string funcName = "";
	bool foundFunc = false;
	std::vector<int> dataConnections;
	int n_cmnds = -1;
	std::vector<Cmnd> cmnds;
	std::vector<FallCmnd> fallCmnds;
	std::string filePath = "";
};

