#include "DumpCDB.h"

DumpCDB::DumpCDB() {}

void DumpCDB::setId(const int id) {
	this->id = id;
}

void DumpCDB::setCmds(const int n) {
	n_cmds = n;
}

void DumpCDB::setFunction(const std::string& name) {
	funcName = name;
}

void DumpCDB::setConnections(const std::vector<int>& con) {
	dataConnections = con;
}

void DumpCDB::printData(void) {
	std::cout << "\nDumpCDB data:";
	std::cout << "\nUnit id:" << id;
	std::cout << "\nFunction: " << funcName;
	std::cout << "\nCommands: " << n_cmds;
	std::cout << "\nConnections: ";
	for (int e : dataConnections) {
		std::cout << e << " ";
	}
}