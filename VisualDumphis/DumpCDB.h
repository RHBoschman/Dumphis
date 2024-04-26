#pragma once

#include <string>
#include <vector>
#include <iostream>

class DumpCDB
{
public:
	DumpCDB();
	void setId(const int _id);
	void setCmds(const int n);
	void setFunction(const std::string& name);
	void setConnections(const std::vector<int>& con);
	void printData(void);

private:
	int id;
	int n_cmds;
	std::string funcName;
	std::vector<int> dataConnections;
};

