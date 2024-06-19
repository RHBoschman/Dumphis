#include "SourceData.h"

void SourceData::setFoundFunction(void) {
	foundFunction = true;
}

void SourceData::setFuncIndex(const int index) {
	funcIndex = index;
}

void SourceData::setFilePath(const std::string& path) {
	filePath = path;
}

void SourceData::setCmndIndex(const int i, const int index) {
	cmnds[i].CmndIndex = index;
}

void SourceData::setFallCmndIndex(const int i, const int j, const int index) {
	cmnds[i].fallCmnds[j].CmndIndex = index;
}

void SourceData::addCmnd(const CMND cmnd) {
	cmnds.push_back(cmnd);
}

void SourceData::printData(void) {
	std::cout << "\nFound: " << (foundFunction ? "true" : "false");
	std::cout << "\nFunc index: " << funcIndex;
	//std::cout << "\nFile path: " << filePath;
	std::cout << "\nCmnds:";
	for (CMND e : cmnds) {
		for (FALL_CMND f : e.fallCmnds) {
			std::cout << "\n\tFALL: " << f.name;
		}
		std::cout << "\n\tCMND: " << e.name;
		std::cout << "\n\tSteps:";
		for (STEP d : e.steps) {
			std::cout << "\n\t\tSTEP: " << d.name;
		}
	}
}

int SourceData::getFuncIndex(void) const {
	return funcIndex;
}

std::vector<CMND> SourceData::getCMNDs(void) {
	return cmnds;
}
