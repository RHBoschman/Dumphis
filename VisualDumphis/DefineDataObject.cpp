#include "DefineDataObject.h"

void DefineDataObject::setName(const std::string& nm) {
	name = nm;
}

void DefineDataObject::setValue(const unsigned int val) {
	value = val;
}

void DefineDataObject::printData(void) const {
	std::cout << "\n" << value << "\t" << name;
}