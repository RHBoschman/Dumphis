#pragma once

#include <string>
#include <iostream>

class DefineDataObject
{
public:
	void setName(const std::string& nm);
	void setValue(const unsigned int val);
	void printData(void) const;

private:
	std::string name;
	unsigned int value;
};

