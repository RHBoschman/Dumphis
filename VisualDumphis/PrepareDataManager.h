#pragma once

#include <vector>
#include "tools\tls.h"
#include "UnitInfoData.h"
#include "DumpCDB.h"
#include "DefineDataObject.h"
#include "SourceData.h"

class PrepareDataManager {
public:
	std::vector<UnitInfoDataObject> unitInfoData;

	PrepareDataManager();
	void createData(std::vector<DumpCDB>* _dumpCDBdata, std::vector<std::string> _funcNames, std::vector<DefineDataObject>* _unitNames, std::vector<SourceData>* _sourceData);

private:
	tls::LogManager log;
	std::vector<DumpCDB>* dumpCDBdata;
	std::vector<DefineDataObject>* unitNames;
	std::vector<SourceData>* sourceData;
	std::vector<std::string> funcNames;
};

