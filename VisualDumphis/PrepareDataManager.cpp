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

	/* NOTE: Code below is NOT right! Whole idea of creating data needs to be different!
	* - What is wrong:
	* I wanted 3 export files: dumphis, iom, unit info. Unit info would contain all function data.
	* BUT: multiple units can (and do) use the SAME function. So, that would result in A LOT
	* of double data. Code below is old and needs to be changed to new approach.
	* 
	* - New approach:
	* Create 4 export files: dumphis, iom, unit info (just with a reference to the function with an id) 
	* and functions (including the reference id). SEE NOTE BOOK PAPER.
	* Probably, the code below can be changed slightly to only hold function data instead of unit data.
	* There should, however, be a separate file called FunctionData.h (like UnitInfoData.h).
	* UnitInfoData.h should also be changed so it only contains the info needed (so without func things, 
	* except it's reference).
	* 
	* In the end, PrepareDataManager should have two 'get' functions, one for unit info and one for function.
	* The other two export files will be retrieved in Manager.cpp directly, since there is no need to change the data.
	*/

	for (SourceData e : *sourceData) {
		UnitInfoDataObject element;
		element.foundFunc = e.getFoundFunction();
		element.filePath = e.getFilePath();
		int funcIndex = e.getFuncIndex();
		element.funcName = (funcIndex >= 0) ? funcNames[funcIndex] : "unknown";

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

		for (DumpCDB dcdb : *dumpCDBdata) {
			if (dcdb.getFuncName() == element.funcName) {
				element.id = dcdb.getId();
				element.n_cmnds = dcdb.getNcmnds();
				element.dataConnections = dcdb.getDataCons();
			}
		}
	}
}
