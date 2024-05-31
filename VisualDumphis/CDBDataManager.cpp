#include "CDBDataManager.h"

CDBDataManager::CDBDataManager() : log("CDBDataManager") {}

void CDBDataManager::createData(const std::string& path) {
	log.logInfo("Start parsing Units/Entities names...");
	tls::File file(path);
	std::string line;

	int index = 0;
	while (file.readLine2(line)) {
		if (containsStr(line, "UNIT_") ||
			containsStr(line, "U_")	||
			containsStr(line, "E_")
			) {
			index++;
			std::string define = "#define";
			size_t pos_define = line.find(define);
			if (pos_define != std::string::npos) {
				std::string substr = line.substr((pos_define + define.length() + 1));

				DefineDataObject element;
				element.setName(getName(substr));
				element.setValue(getNumber(substr));
				
				data.push_back(element);
			}
			else {
				log.logError("'#define' not found");
			}
		}
	}

	log.logInfo(std::format("Found {} units/entities", index));

#if PRINT_ALL
	for (DefineDataObject e : data) {
		e.printData();
	}
#endif
}
