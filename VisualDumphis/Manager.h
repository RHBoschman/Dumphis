#pragma once

#include <iostream>
#include <filesystem>
#include <format>
#include "tools/tls.h"
#include "DumpCDBManager.h"
#include "DumpHisManager.h"
#include "CDBDataManager.h"
#include "IOMDataManager.h"
#include "SourceDataManager.h"
#include "PrepareDataManager.h"

/* TODO
* - Verify project path input
* - Find and parse cdb_data.h
* - Create data with CDBDataManger (as CDBData objects)
*/

namespace fs = std::filesystem;

class Manager
{
public:
	Manager(void);
	void start(void);

private:
	DumpCDBManager dumpcdbRead;
	DumpHisManager dumphisRead;
	CDBDataManager cdbdataRead;
	IOMDataManager iomnameRead;
	SourceDataManager sourceDataRead;
	PrepareDataManager prepareData;

	tls::LogManager log;
	tls::Gui gui;
	fs::path projectPath;
	fs::path snapshotPath;
	std::string file_dumpcdb = "dumpcdb.csv";
	std::string file_dumphis = "dumphis.csv";
	std::string file_cdb_data = "global\\include\\cdb_data.h";
	std::string file_iom_name = "zip_rel\\boot\\IOM_NAME.PP";
	std::string ctlRoot = "Bachmann\\ctl";

	void askForInput(void);
	bool verifyInput(const std::string& proj, const std::string& file);
	void readFiles(void);
	bool doesFileExist(const fs::path& root, const std::string& relativePath);
};

