#include "PrepareDataManager.h"

PrepareDataManager::PrepareDataManager() : log("PrepareDataManager") {}

void PrepareDataManager::createData(
	std::vector<DumpCDB>* _dumpCDBdata, 
	std::vector<std::string> _funcNames,
	std::vector<DefineDataObject>* _unitNames, 
	std::vector<SourceData>* _sourceData 
	) {
	dumpCDBdata = _dumpCDBdata;
	unitNames = _unitNames;
	sourceData = _sourceData;
	funcNames = _funcNames;

	// Creating data for function export file
	for (SourceData e : *sourceData) {
		FunctionDataObject element;
		element.found = e.getFoundFunction();
		element.filePath = e.getFilePath();
		int funcIndex = e.getFuncIndex();
		element.funcId = funcIndex;
		element.name = (funcIndex >= 0) ? funcNames[funcIndex] : "unknown";

		for (CMND c : e.getCMNDs()) {
			Cmnd cmnd;
			cmnd.id = c.CmndIndex;
			cmnd.name = c.name;

			for (STEP s : c.steps) {
				Step step;
				step.name = s.name;
				cmnd.steps.push_back(step);
			}

			for (FALL_CMND fc : c.fallCmnds) {
				FallCmnd fallCmnd;
				fallCmnd.id = fc.CmndIndex;
				fallCmnd.name = fc.name;
				fallCmnd.refCmndId = c.CmndIndex;
				element.fallCmnds.push_back(fallCmnd);
			}

			element.cmnds.push_back(cmnd);
		}
		functionData.push_back(element);
	}

	// Creating data for unit info export file
	for (DumpCDB dcdb : *dumpCDBdata) {
		UnitInfoDataObject element;
		element.id = dcdb.getId();
		element.n_cmnds = dcdb.getNcmnds();
		element.dataConnections = dcdb.getDataCons();

		for (int i = 0; i < funcNames.size(); i++) {
			if (funcNames[i] == dcdb.getFuncName()) {
				element.funcId = i;
				break;
			}
		}

		for (DefineDataObject e : *unitNames) {
			if (e.getValue() == dcdb.getId()) {
				element.name = e.getName();
				break;
			}
		}

		if (element.name[0] == 'E')
			element.type = ENTITY;
		else if (element.name[0] == 'U')
			element.type = UNIT;

		unitInfoData.push_back(element);
	}

	log.logInfo("Done preparing data");
}

std::vector<UnitInfoDataObject>* PrepareDataManager::getUnitInfoData(void) {
	return &unitInfoData;
}

std::vector<FunctionDataObject>* PrepareDataManager::getFunctionData(void) {
	return &functionData;
}
