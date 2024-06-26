#pragma once

#include <vector>
#include <string>

enum UnitTypes {
	UNIT,
	ENTITY,
	UNKNOWN
};

struct UnitInfoDataObject {
	int id = -1;
	int funcId = -1;
	UnitTypes type = UNKNOWN;
	std::string name = "";
	std::vector<int> dataConnections;
	int n_cmnds = -1;
};

