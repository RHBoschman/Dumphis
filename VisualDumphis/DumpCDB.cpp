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
	std::cout << "\n\tUnit id: " << id;
	std::cout << "\n\tFunction: " << funcName;
	std::cout << "\n\tCommands: " << n_cmds;
	std::cout << "\n\tConnections: ";
	for (int e : dataConnections) {
		std::cout << e << " ";
	}
}

std::string DumpCDB::getFuncName(void) {
	return funcName;
}