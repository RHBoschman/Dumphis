#pragma once

#include <vector>
#include "tools\tls.h"
#include "UnitInfoData.h"
#include "FunctionData.h"
#include "DumpCDB.h"
#include "DefineDataObject.h"
#include "SourceData.h"

class PrepareDataManager {
public:
	PrepareDataManager();
	void createData(std::vector<DumpCDB>* _dumpCDBdata, std::vector<std::string> _funcNames, std::vector<DefineDataObject>* _unitNames, std::vector<SourceData>* _sourceData);
	std::vector<UnitInfoDataObject>* getUnitInfoData(void);
	std::vector<FunctionDataObject>* getFunctionData(void);

private:
	tls::LogManager log;
	std::vector<DumpCDB>* dumpCDBdata;
	std::vector<DefineDataObject>* unitNames;
	std::vector<SourceData>* sourceData;
	std::vector<std::string> funcNames;

	std::vector<UnitInfoDataObject> unitInfoData;
	std::vector<FunctionDataObject> functionData;
};

