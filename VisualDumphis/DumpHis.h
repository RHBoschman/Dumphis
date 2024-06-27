#pragma once

#include <string>
#include <iostream>

class DumpHis
{
public:
	void setTime(const std::string& t);
	void setUnitId(const int n);
	void setCmdId(const int n);
	void setStepId(const int n);
	void setState(const int n);
	void setAlarmId(const int n);
	void printData(void);
	std::string getTime(void) const;
	int getUnitId(void) const;
	int getCmdId(void) const;
	int getStepId(void) const;
	int getUnitState(void) const;
	int getAlarmId(void) const;

private:
	std::string time;
	int unitId;
	int cmdId;
	int stepId;
	int unitState;
	int alarmId;
};

