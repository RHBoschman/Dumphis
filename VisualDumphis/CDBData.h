#pragma once

#include <string>
#include <iostream>

class CDBData
{
public:
	void setName(const std::string& nm);
	void setValue(const int val);
	void printData(void) const;

private:
	std::string name;
	int value;
};

