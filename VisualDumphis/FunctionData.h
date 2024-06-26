#pragma once

#include <vector>
#include <string>

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

struct FunctionDataObject {
	int funcId = -1;
	bool found = false;
	std::string name = "";
	std::string filePath = "";
	std::vector<Cmnd> cmnds;
	std::vector<FallCmnd> fallCmnds;
};