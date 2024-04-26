#include "DumpCDBManager.h"

void DumpCDBManager::createData(const std::string& path) {
	tls::File file(path);
	std::string line;
	while (file.readLine2(line)) {
		DumpCDB element; //NOT GOOD: should only create element on finding "CDB"
		int n_cmds = 0;

		if (containsStr(line, "CBD"))
			element.setId(getE_CDB(line));
		else if (containsStr(line, "FUNCTION"))
			element.setFunction(getE_FUNCTION(line));
		else if (containsStr(line, "DATA"))
			element.setConnections(getE_DATA(line));
		else if (containsStr(line, "COMMAND"))
			n_cmds++;

		element.setCmds(n_cmds);
	}
}

int DumpCDBManager::getE_CDB(const std::string& line) {

}

std::string& DumpCDBManager::getE_FUNCTION(const std::string& line) {

}

std::vector<int> DumpCDBManager::getE_DATA(const std::string& line) {

}

bool DumpCDBManager::containsStr(const std::string& line, const std::string& str) {

}

std::string& DumpCDBManager::getAfterComma(const std::string& line) {

}