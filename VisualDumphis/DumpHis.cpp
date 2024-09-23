#include "DumpHis.h"

void DumpHis::setHisId(const std::string& id) {
	hisId = id;
}

void DumpHis::setTime(const std::string& t) {
	time = t;
}

void DumpHis::setUnitId(const int n) {
	unitId = n;
}

void DumpHis::setCmdId(const int n) {
	cmdId = n;
}

void DumpHis::setStepId(const int n) {
	stepId= n;
}

void DumpHis::setState(const int n) {
	unitState = n;
}

void DumpHis::setAlarmId(const int n) {
	alarmId = n;
}

void DumpHis::printData(void) {
	std::cout << "\nDumphis data:";
	std::cout << "\nTime: " << time;
	std::cout << "\tUnit: " << unitId;
	std::cout << "\tCmd: " << cmdId;
	std::cout << "\tStep: " << stepId;
	std::cout << "\tState: " << unitState;
	std::cout << "\tAlarm: " << alarmId;
}

std::string DumpHis::getHisId(void) const {
	return hisId;
}

std::string DumpHis::getTime(void) const {
	return time;
}

int DumpHis::getUnitId(void) const {
	return unitId;
}

int DumpHis::getCmdId(void) const {
	return cmdId;
}

int DumpHis::getStepId(void) const {
	return stepId;
}

int DumpHis::getUnitState(void) const {
	return unitState;
}

int DumpHis::getAlarmId(void) const {
	return alarmId;
}
