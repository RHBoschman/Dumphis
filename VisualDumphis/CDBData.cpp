#include "CDBData.h"

void CDBData::setName(const std::string& nm) {
	name = nm;
}

void CDBData::setValue(const int val) {
	value = val;
}

void CDBData::printData(void) const {
	std::cout << "\n" << name << "\t\t\t" << value;
}