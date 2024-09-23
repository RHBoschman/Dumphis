#include "JSONExportManager.h"

JSONExportManager::JSONExportManager() : log("JSONExportManager") {}

void JSONExportManager::exportUnitInfoData(const std::vector<UnitInfoDataObject>* unitInfoData) {
	json jsonObj = json::array();

	for (const UnitInfoDataObject& e : *unitInfoData) {
		json unit = {
			{"id", e.id},
			{"funcId", e.funcId},
			{"type", (int)e.type},
			{"name", e.name},
			{"n_cmnds", e.n_cmnds},
			{"dataConnections", e.dataConnections}
		};
		jsonObj.push_back(unit);
	}

	exportFile("UnitInfoData.json", jsonObj);
}

void JSONExportManager::exportFunctionData(const std::vector<FunctionDataObject>* functionData) {
	json jsonObj = json::array();

	for (const FunctionDataObject& e : *functionData) {
		json function = {
			{"funcId", e.funcId},
			{"found", e.found},
			{"name", e.name},
			{"filePath", e.filePath},
			{"cmnds", json::array()},
			{"fallCmnds", json::array()}
		};

		for (const Cmnd& c : e.cmnds) {
			json cmnd = {
				{"id", c.id},
				{"name", c.name},
				{"steps", json::array()}
			};

			for (const Step& s : c.steps) {
				json step = {
					{"name", s.name}
				};
				cmnd["steps"].push_back(step);
			}
			function["cmnds"].push_back(cmnd);
		}

		for (const FallCmnd& fc : e.fallCmnds) {
			json fallCmnd = {
				{"id", fc.id},
				{"name", fc.name},
				{"refCmndId", fc.refCmndId}
			};
			function["fallCmnds"].push_back(fallCmnd);
		}

		jsonObj.push_back(function);
	}

	exportFile("FunctionData.json", jsonObj);
}

void JSONExportManager::exportDumphisData(const std::vector<DumpHis>* dumpHisData) {
	json jsonObj = json::array();

	for (const DumpHis& e : *dumpHisData) {
		json dump = {
			{"ID", e.getHisId()},
			{"time", e.getTime()},
			{"unitId", e.getUnitId()},
			{"cmdId", e.getCmdId()},
			{"stepId", e.getStepId()},
			{"unitState", e.getUnitState()},
			{"alarmId", e.getAlarmId()}
		};
		jsonObj.push_back(dump);
	}

	exportFile("DumphisData.json", jsonObj);
}

void JSONExportManager::exportIomData(const std::vector<DefineDataObject>* iomData) {
	json jsonObj = json::array();

	for (const DefineDataObject& e : *iomData) {
		json iom = {
			{"name", e.getName()},
			{"value", e.getValue()},
		};
		jsonObj.push_back(iom);
	}

	exportFile("IOMData.json", jsonObj);
}

void JSONExportManager::exportFile(const std::string fileName, const json& obj) {
	std::string path = "..\\exports\\" + fileName;
	std::ofstream outFile(path);
	if (outFile.is_open()) {
		outFile << obj.dump(INDENTATION_LEVEL);
		outFile.close();
		log.logInfo(std::format("Created export file: {}", path));
	}
	else {
		log.logFatal(std::format("Unable to open export file: {}. Application will terminate", path));
		exit(0);
	}
}