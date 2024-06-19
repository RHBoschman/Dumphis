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

std::string DefineDataObject::getName(void) const {
	return name;
}

int DefineDataObject::getValue(void) const {
	return (int)value;
}