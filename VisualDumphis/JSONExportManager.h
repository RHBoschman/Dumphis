#pragma once

#include "tools/tls.h"
#include "UnitInfoData.h"
#include "FunctionData.h"
#include "DumpHis.h"
#include "DefineDataObject.h"
#include "nlohmann/json.hpp"

#include <vector>
#include <fstream>
#include <format>

#define INDENTATION_LEVEL 4

using namespace nlohmann;

class JSONExportManager {
public:
	JSONExportManager();
	void exportUnitInfoData(const std::vector<UnitInfoDataObject>* unitInfoData);
	void exportFunctionData(const std::vector<FunctionDataObject>* functionData);
	void exportDumphisData(const std::vector<DumpHis>* dumpHisData);
	void exportIomData(const std::vector<DefineDataObject>* iomData);

private:
	tls::LogManager log;

	void exportFile(const std::string fileName, const json& obj);
};

